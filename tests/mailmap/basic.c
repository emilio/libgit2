#include "clar.h"
#include "clar_libgit2.h"

#include "common.h"
#include "git2/mailmap.h"

static git_mailmap *mailmap = NULL;

const char TEST_MAILMAP[] =
	"Foo bar <foo@bar.com> <foo@baz.com>\n"
	"Blatantly invalid line\n"
	"Foo bar <foo@bar.com> <foo@bal.com>\n";

void test_mailmap_basic__initialize(void)
{
	cl_git_pass(git_mailmap_parse(&mailmap, TEST_MAILMAP, sizeof(TEST_MAILMAP)));
}

void test_mailmap_basic__cleanup(void)
{
	if (mailmap) {
		git_mailmap_free(mailmap);
		mailmap = NULL;
	}
}

void test_mailmap_basic__entry(void)
{
	git_mailmap_entry* entry;

	cl_assert(git_mailmap_entry_count(mailmap) == 2);

	entry = git_mailmap_entry_byindex(mailmap, 0);
	cl_assert(entry);
	cl_assert(!entry->replace_name);
	cl_assert(!git__strcmp(entry->replace_email, "foo@baz.com"));

	entry = git_mailmap_entry_byindex(mailmap, 10000);
	cl_assert(!entry);
}

void test_mailmap_basic__lookup_not_found(void)
{
	git_mailmap_entry* entry = git_mailmap_entry_lookup(
		mailmap,
		"Whoever",
		"doesnotexist@fo.com");
	cl_assert(!entry);
}

void test_mailmap_basic__lookup(void)
{
	git_mailmap_entry* entry = git_mailmap_entry_lookup(
		mailmap,
		"Typoed the name once",
		"foo@baz.com");
	cl_assert(entry);
	cl_assert(!strcmp(entry->real_name, "Foo bar"));
}
