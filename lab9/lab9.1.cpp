#include <stdio.h>
#include <sys/acl.h>
#include <acl/libacl.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#define TEST_FILE "./cat-says"

int main(void) {
  acl_t file_info = acl_get_file(TEST_FILE, ACL_TYPE_ACCESS);

  char  *text_acl;
	ssize_t len;

  text_acl = acl_to_text (file_info, &len);

	if (text_acl == NULL) {
		perror ("acl_to_text()");
	}

	printf ("Первоначальный ACL: \n%s\n", text_acl);

  int age;

  acl_entry_t entry_p;

  int entry_id = ACL_FIRST_ENTRY;

  do {
    age = acl_get_entry(file_info, entry_id, &entry_p);

    acl_tag_t tag_type_p;

    acl_get_tag_type(entry_p, &tag_type_p);

    if(tag_type_p == ACL_USER_OBJ){
      acl_permset_t permset_p;

      acl_get_permset(entry_p, &permset_p);

      printf ("Добавляем пользователю возможность записи\n");
      
      acl_add_perm(permset_p, ACL_WRITE);

      acl_set_permset(entry_p, permset_p);
    }

    entry_id = ACL_NEXT_ENTRY;
  } while(age != 0);

  acl_calc_mask(&file_info);

  acl_valid(file_info);

  acl_set_file(TEST_FILE, ACL_TYPE_ACCESS, file_info);

  text_acl = acl_to_text (file_info, &len);

	if (text_acl == NULL) {
		perror ("acl_to_text()");
	}

	printf ("ACL после манипуляций: \n%s\n", text_acl);

  acl_free(file_info);
}
