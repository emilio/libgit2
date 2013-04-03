/*
 * Copyright (C) the libgit2 contributors. All rights reserved.
 *
 * This file is part of libgit2, distributed under the GNU GPL v2 with
 * a Linking Exception. For full terms see the included COPYING file.
 */
#ifndef INCLUDE_cache_h__
#define INCLUDE_cache_h__

#include "git2/common.h"
#include "git2/oid.h"
#include "git2/odb.h"

#include "thread-utils.h"
#include "oidmap.h"

enum {
	GIT_CACHE_STORE_ANY = 0,
	GIT_CACHE_STORE_RAW = 1,
	GIT_CACHE_STORE_PARSED = 2
};

typedef struct {
	git_oid oid;
	git_atomic refcount;
	uint32_t cache_size;
	uint32_t flags;
} git_cached_obj;

typedef struct {
	git_oidmap *map;
	git_mutex lock;
} git_cache;

int git_cache_init(git_cache *cache);
void git_cache_free(git_cache *cache);

void *git_cache_store_raw(git_cache *cache, git_odb_object *entry);
void *git_cache_store_parsed(git_cache *cache, git_object *entry);

git_odb_object *git_cache_get_raw(git_cache *cache, const git_oid *oid);
git_object *git_cache_get_parsed(git_cache *cache, const git_oid *oid);
void *git_cache_get_any(git_cache *cache, const git_oid *oid);

GIT_INLINE(void) git_cached_obj_incref(void *_obj)
{
	git_cached_obj *obj = _obj;
	git_atomic_inc(&obj->refcount);
}

void git_cached_obj_decref(void *_obj);

#endif
