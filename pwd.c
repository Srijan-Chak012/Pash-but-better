#include "headers.h"
#include "pwd.h"

void present_working_directory() {
    getcwd(current_work_dir, 4000);
    printf("%s\n", current_work_dir);
}