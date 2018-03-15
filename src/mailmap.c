/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */

#include "git2/mailmap.h"

#include "blob.h"
#include "commit.h"
#include "git2/common.h"
#include "git2/repository.h"
#include "git2/revparse.h"
#include "git2/sys/commit.h"

struct git_mailmap {
	git_array_t(char*) lines;
};

static void git_mailmap_parse_line(
	git_mailmap* mailmap,
	const char* contents,
	size_t size)
{
	if (!size)
		return;
	if (contents[0] == '#')
		return;
}

static void git_mailmap_parse(
	git_mailmap* mailmap,
	const char* contents,
	size_t size)
{
	size_t start = 0;
	size_t i;
	for (i = 0; i < size; ++i)
		if (contents[i] != '\n')
			continue;
		git_mailmap_parse_line(mailmap, contents + start, i - start);
		start = i + 1;
	}
}

int git_mailmap_create(git_mailmap** mailmap, git_repository* repo)
{
	git_commit* head = NULL;
	git_blob* mailmap_blob = NULL;
	git_off_t size = 0;
	const char* contents = NULL;
	int ret;

	*mailmap = git__malloc(sizeof(struct git_mailmap));
	git_array_init((*mailmap)->lines);

	ret = git_revparse_single((git_object **)&head, repo, "HEAD");
	if (ret)
		goto error;

	ret = git_object_lookup_bypath(
			(git_object**) &mailmap_blob,
			(const git_object*) head,
			".mailmap",
			GIT_OBJ_BLOB);
	if (ret)
		goto error;

	size = git_blob_rawsize(mailmap_blob);
	contents = git_blob_rawcontent(mailmap_blob);

	(void) size;
	printf(contents);

	return 0;

error:
	assert(ret);

	if (mailmap_blob)
		git_blob_free(mailmap_blob);
	if (head)
		git_commit_free(head);
	git_mailmap_free(*mailmap);
	return ret;
}

void git_mailmap_free(struct git_mailmap* mailmap)
{
	size_t i;
	char* line;
	for (i = 0; i < git_array_size(mailmap->lines); i++) {
		line = *git_array_get(mailmap->lines, i);
		git__free(line);
	}

	git_array_clear(mailmap->lines);
	git__free(mailmap);
}
