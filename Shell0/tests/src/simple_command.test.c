#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cmocka.h"
#include "../../include/simple_command.h"

static int setup(void **state) {
  simple_command* sc = simple_command_create();
  if(sc == NULL) {
    return -1;
  }

  *state = sc;
  return 0;
}

static int teardown(void **state) {
  simple_command_destroy(*state);
  return 0;
}

static void test_simple_command_create(void **state) {
    simple_command* sc = *state;
    assert_string_equal(sc->name, "");
    assert_null(sc->argv);
    assert_int_equal(sc->exit_status, -1);
    assert_int_equal(sc->redirections->size, 0);
}

static void test_simple_command_add_redirection(void **state) {
    simple_command* sc = *state;
    simple_command_add_redirection(sc, "> file.txt");
    simple_command_add_redirection(sc, "2>&1");
    assert_string_equal("> file.txt", (char*) sc->redirections->head->data);
    assert_string_equal("2>&1", (char*) sc->redirections->head->next->data);
}

static void test_simple_command_get_next_redirection_intent(void **state){
  simple_command* sc = *state;
  simple_command_add_redirection(sc, "> file.txt");
  simple_command_add_redirection(sc, "2>&1");
  char* ri = simple_command_get_next_redirection_intent(sc);
  char* ri2 = simple_command_get_next_redirection_intent(sc);
  char* ri3 = simple_command_get_next_redirection_intent(sc);
  assert_non_null(ri);
  assert_non_null(ri2);
  assert_string_equal("> file.txt", ri);
  assert_string_equal("2>&1", ri2);
  assert_null(ri3);
}

static void test_simple_command_destroy(void **state) {
  simple_command* sc = *state;
  simple_command_add_redirection(sc, "> file.txt");
  simple_command_add_redirection(sc, "2>&1");
  simple_command_destroy(sc);
}

static void test_simple_command_from_string(void **state){
  simple_command * sc = simple_command_from_string("LC=en HOME='/root' ls -ali / >> file.txt <file2.txt 2>&1");

  assert_int_equal(sc->nb_assignments, 2);
  assert_string_equal("LC=en", sc->env_assignements[0]);
  assert_string_equal("HOME=/root", sc->env_assignements[1]);
  assert_string_equal(sc->name, "ls");
  assert_string_equal("ls", sc->argv[0]);
  assert_string_equal("-ali", sc->argv[1]);
  assert_string_equal("/", sc->argv[2]);

  assert_string_equal(">>file.txt", simple_command_get_next_redirection_intent(sc));
  assert_string_equal("<file2.txt", simple_command_get_next_redirection_intent(sc));
  assert_string_equal("2>&1", simple_command_get_next_redirection_intent(sc));
  assert_int_equal(sc->argc, 3);

  simple_command_destroy(sc);

  simple_command * sc2 = simple_command_from_string("2>&1 ls -a >file /home");

  assert_string_equal(sc2->name, "ls");
  assert_string_equal("ls", sc2->argv[0]);
  assert_string_equal("-a", sc2->argv[1]);
  assert_string_equal("/home", sc2->argv[2]);
  assert_string_equal("2>&1", simple_command_get_next_redirection_intent(sc2));
  assert_string_equal(">file", simple_command_get_next_redirection_intent(sc2));
  assert_int_equal(sc2->argc, 3);

  simple_command_destroy(sc2);

  simple_command * sc3 = simple_command_from_string("ls -ali \"my folder with spaces in the name \" /home");

  assert_string_equal(sc3->name, "ls");
  assert_string_equal("ls", sc3->argv[0]);
  assert_string_equal("-ali", sc3->argv[1]);
  assert_string_equal("my folder with spaces in the name ", sc3->argv[2]);
  assert_string_equal("/home", sc3->argv[3]);
  assert_int_equal(sc3->argc, 4);
  simple_command_destroy(sc3);
}

const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup_teardown(test_simple_command_create, setup, teardown),
  cmocka_unit_test_setup_teardown(test_simple_command_add_redirection, setup, teardown),
  cmocka_unit_test_setup_teardown(test_simple_command_get_next_redirection_intent, setup, teardown),
  cmocka_unit_test(test_simple_command_from_string),
  cmocka_unit_test_setup(test_simple_command_destroy, setup)
};

int main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
