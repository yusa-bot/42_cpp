# cpp-module-04 tests
## How to reproduce "crash" behavior by `double delete`
- Compile the program by
```shell
$ c++ -std=c++98 -Wall -Werror -Wextra -g -fsanitize=address double_delete_dog.cpp ../ex01/Animal.cpp ../ex01/Dog.cpp ../ex01/Brain.cpp -o double_delete_dog
$ ./double_delete_dog
```
- This should be showing logs like bellow

```shell
[TEST] Reproduce double-delete risk in Dog::operator=
Animal default constructor called
Dog default constructor called
Brain default constructor called
Animal default constructor called
Dog default constructor called
Brain default constructor called
Dog copy assignment operator called
Animal copy assignment operator called
Brain destructor called
[INFO] Caught expected std::bad_alloc during assignment
[INFO] Leaving scope. If _brain is dangling, destructor may double-delete.
Dog destructor called
Brain destructor called
AddressSanitizer:DEADLYSIGNAL
=================================================================
==74899==ERROR: AddressSanitizer: SEGV on unknown address 0xfffffffffffffffe (pc 0xaaaad9b87bd0 bp 0xfffff39e5a40 sp 0xfffff39e51f0 T0)
==74899==The signal is caused by a READ memory access.
==74899==WARNING: invalid path to external symbolizer!
==74899==WARNING: Failed to use and restart external symbolizer!
    #0 0xaaaad9b87bd0  (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0xd7bd0) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)
    #1 0xaaaad9ad6a60  (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0x26a60) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)
    #2 0xaaaad9b4c774  (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0x9c774) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)
    #3 0xaaaad9b85cd4  (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0xd5cd4) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)
    #4 0xaaaad9b87798  (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0xd7798) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)
    #5 0xaaaad9b87134  (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0xd7134) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)
    #6 0xaaaad9b8612c  (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0xd612c) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)
    #7 0xffff9dfd73fc  (/lib/aarch64-linux-gnu/libc.so.6+0x273fc) (BuildId: 9f32d43c341bff10b9e7196738eedcfc4f3cc36c)
    #8 0xffff9dfd74d4  (/lib/aarch64-linux-gnu/libc.so.6+0x274d4) (BuildId: 9f32d43c341bff10b9e7196738eedcfc4f3cc36c)
    #9 0xaaaad9ad57ec  (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0x257ec) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV (/workspaces/agentic-review/ayusa--cpp-module-04/tests/double_delete_dog+0xd7bd0) (BuildId: b7ad737ad1651334ce8e87b9eec4ab1ff4643db1)
==74899==ABORTING
```