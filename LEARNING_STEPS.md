# Building this yourself, one step at a time

This reference implementation was assembled to be easier to study and reproduce than the larger banking demo. **Reading and understanding code is not the same as writing it independently.** Before treating it as your own resume work, try recreating the modules yourself without looking at the solution for every line.

## Five stages

1. **InputHandling**: Write `validUsername`, `validPassword` and `parseMoney` from a blank file. Explain `find()`, `substr()`, loops, and integer cents. Add 3 test cases.
2. **Account**: Write the fields, both constructors, getters, `deposit` and `withdraw`. Explain why negative amounts must be rejected and why the loaded constructor uses the old salt/hash.
3. **Crypto**: Reproduce `generateSalt` and `hashPassword` by consulting OpenSSL documentation, not from memory. Understand every argument to `PKCS5_PBKDF2_HMAC`. Write the comparison and hex helpers.
4. **AccountManager**: Write the vector search, `createAccount`, then simple load/save. Trace one account as it is saved, reloaded, and authenticated. Explain the temporary-file rename at a high level.
5. **main**: Write the menu by yourself, call the methods from the first four modules, then add a unit test and an end-to-end demo run.

## Questions you should be able to answer

- What is stored on disk, and why isn't the raw password there?
- Why is the salt generated when an account is created but reused when an account is loaded?
- What is the difference between `Account*` and an owned `Account` inside a `std::vector`?
- How does `parseMoney("125.5")` become 12,550 cents?
- What happens if saving a deposit fails?
- Which limitations make this an educational demo instead of a real banking system?

If you cannot explain or recreate a section, keep it as a learning reference until you can. Keep the earlier, more sophisticated version archived as a historical artifact rather than deleting your learning history.
