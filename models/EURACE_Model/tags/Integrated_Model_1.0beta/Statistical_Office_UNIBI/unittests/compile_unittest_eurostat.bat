gcc -D_DEBUG_MODE -g -I../../libmboard/include -L../../libmboard/lib ../../memory.c ../../rules.c ../../messageboards.c ../Eurostat_Functions.c ../Eurostat_aux_functions.c Eurostat_unittests.c unittest_suite_eurostat.c -lm -lcunit -lmboard_s 