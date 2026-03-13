#include <assert.h>
#include <stdbool.h>

bool pdelete_has_password(const char *pwd);
bool pdelete_missing_password_argument(const char *pwd, const char *arg);

static void test_pdelete_has_password(void)
{
    assert(pdelete_has_password("hash"));
    assert(!pdelete_has_password(""));
    assert(!pdelete_has_password((const char *)0));
}

static void test_pdelete_missing_password_argument(void)
{
    assert(pdelete_missing_password_argument("hash", ""));
    assert(pdelete_missing_password_argument("hash", (const char *)0));
    assert(!pdelete_missing_password_argument("hash", "guess"));
    assert(!pdelete_missing_password_argument("", ""));
    assert(!pdelete_missing_password_argument((const char *)0, ""));
}

int main(void)
{
    test_pdelete_has_password();
    test_pdelete_missing_password_argument();
    return 0;
}
