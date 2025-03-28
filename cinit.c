#include <stdio.h>
#define NOB_IMPLEMENTATION
#include "nob.h"

#define PROJECTS_ROOT "/Users/main/Documents/perso.nosync"

bool copy_to_proj(const char *file, const char *path) {
  const char *src_nob_path =
      nob_temp_sprintf("%s/cinit/%s", PROJECTS_ROOT, file);
  const char *dst_nob_path = nob_temp_sprintf("%s/%s", path, file);
  if (!nob_copy_file(src_nob_path, dst_nob_path)) {
    fprintf(stderr, "Could not copy %s to %s\n", src_nob_path, dst_nob_path);
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  const char *prog_name = nob_shift(argv, argc);
  if (argc) {
    const char *project_name = nob_shift(argv, argc);
    const char *path = nob_temp_sprintf("%s/%s", PROJECTS_ROOT, project_name);
    if (!nob_mkdir_if_not_exists(path)) {
      fprintf(stderr, "Could not create directory: %s\n", path);
      return EXIT_FAILURE;
    }
    printf("Succesfully created %s\n", path);

    if (!copy_to_proj("nob.h", path)) {
      return EXIT_FAILURE;
    }
    if (!copy_to_proj("nob.c", path)) {
      return EXIT_FAILURE;
    }
    if (!copy_to_proj(".gitignore", path)) {
      return EXIT_FAILURE;
    }
    if (!copy_to_proj("template.c", path)) {
      return EXIT_FAILURE;
    }

    const char *src_template = nob_temp_sprintf("%s/template.c", path);
    const char *dst_template = nob_temp_sprintf("%s/%s.c", path, project_name);
    if (!nob_rename(src_template, dst_template)) {
      fprintf(stderr, "Could not rename %s into %s\n", src_template,
              dst_template);
      return EXIT_FAILURE;
    }

    if (!nob_set_current_dir(path)) {
      fprintf(stderr, "Could not move to %s\n", path);
      return EXIT_FAILURE;
    }

    Nob_Cmd cmd = {0};

    nob_cmd_append(&cmd, "git", "init");
    if (!nob_cmd_run_sync_and_reset(&cmd)) {
      fprintf(stderr, "Failed to init git repository.\n");
      return EXIT_FAILURE;
    }

    nob_cmd_append(&cmd, "sed", "-i.bak", "-e",
                   nob_temp_sprintf("s/cinit/%s/g", project_name), ".gitignore",
                   "nob.c", nob_temp_sprintf("%s.c", project_name));
    if (!nob_cmd_run_sync_and_reset(&cmd)) {
      fprintf(stderr, "Could not replace project name.\n");
      return EXIT_FAILURE;
    }
    if (!nob_delete_file(nob_temp_sprintf("%s.c.bak", project_name))) {
      fprintf(stderr, "Could not remove `%s.c.bak`.\n", project_name);
    }
    if (nob_delete_file("nob.c.bak")) {
      fprintf(stderr, "Could not remove `nob.c.bak`.\n");
    }
    if (nob_delete_file(".gitignore.bak")) {
      fprintf(stderr, "Could not remove `.gitignore.bak`.\n");
    }

    nob_cmd_append(&cmd, "cc", "-o", "nob", "nob.c");
    if (!nob_cmd_run_sync_and_reset(&cmd)) {
      fprintf(stderr, "Failed to compile `nob.c`.\n");
      return EXIT_FAILURE;
    }

    nob_cmd_append(&cmd, "./nob");
    if (!nob_cmd_run_sync_and_reset(&cmd)) {
      fprintf(stderr, "Failed to compile `%s.c`.\n", project_name);
      return EXIT_FAILURE;
    }

    printf("Project creation successful!\n\n");
    printf("Execute the project with:\n\n`cd %s`\n`./nob && ./%s\n", path,
           project_name);
    system(nob_temp_sprintf("cd %s", path));
  } else {
    fprintf(stderr, "Usage: %s <project_name>\n", prog_name);
    return EXIT_FAILURE;
  }
}
