gcc -D_DEBUG_MODE -g -I../../libmboard/include -L../../libmboard/lib unittest.c ../../memory.c ../../rules.c ../../messageboards.c ../../my_library_functions.c ../Firm_Financial_Management_Functions.c Firm_financial_management_unittests.c Firm_Financial_Management_unittests_new.c -lm -lcunit -lmboard_sd 