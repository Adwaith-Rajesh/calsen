## .calsenignore

allow the user to ignore files and folder from being indexed

- \* -> matches 0 to more characters
- ? -> matches 1 character

## rules

- "test_file" will match the literal "test_file"

  1. this mean that "test_file" won't match "path/to/test_file"

- "\*test_file" will match any path / string that ends in "test_file"

  1. "\*test_file" will match "path/to/test_file"
  2. "\*test_file" won't match "path/to/test_file/some_file"

- "test_file\*" will match any string / path that begins with "test_file"

  1. "test_file\*" will match "test_file/some/path"
  2. "\*test_file" won't match "some/path/test_file"

- "\*test_file\*" will match any string / path that contain the string "test_file"

- "test\*file" will match any path begin and ends test and file respectively.

You can mix these match expressions to make you own logic
