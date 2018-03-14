#include "clar_libgit2.h"
#include "buffer.h"

static void expect_decode_pass(const char *expected, const char *encoded)
{
	git_buf buf = GIT_BUF_INIT;

	cl_git_pass(git_buf_decode_percent(&buf, encoded, strlen(encoded)));

	cl_assert_equal_s(expected, git_buf_cstr(&buf));
	cl_assert_equal_i(strlen(expected), git_buf_len(&buf));

	git_buf_free(&buf);
}

void test_buf_percent__decode_succeeds(void)
{
	expect_decode_pass("", "");
	expect_decode_pass("a", "a");
	expect_decode_pass("github.com", "github.com");
	expect_decode_pass("github.com", "githu%62.com");
	expect_decode_pass("github.com", "github%2ecom");
	expect_decode_pass("foo bar baz", "foo%20bar%20baz");
}

void test_buf_percent__ignores_invalid(void)
{
	expect_decode_pass("githu%%.com", "githu%%.com");
	expect_decode_pass("github.co%2", "github.co%2");
	expect_decode_pass("github%2.com", "github%2.com");
	expect_decode_pass("githu%2z.com", "githu%2z.com");
	expect_decode_pass("github.co%9z", "github.co%9z");
	expect_decode_pass("github.co%2", "github.co%2");
	expect_decode_pass("github.co%", "github.co%");
}
