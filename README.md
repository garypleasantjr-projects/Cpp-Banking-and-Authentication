# C++ Banking and Authentication System

**Personal project | C++17 | OpenSSL | CMake | CTest**

A modular command-line banking simulation built in C++17. The application manages user accounts, verifies credentials, processes deposits and withdrawals, and persists account data between sessions. It combines object-oriented design, file I/O, input validation, and password-based authentication in a single console application.

## Features

- **Account management:** Create accounts, authenticate users, and change passwords after verifying the current password.
- **Transactions:** View balances, deposit funds, and withdraw funds with insufficient-funds and overflow checks.
- **Password protection:** Generate random per-account salts and derive password hashes using OpenSSL PBKDF2-HMAC-SHA256. Compare derived hashes with a constant-time comparison function.
- **Persistent storage:** Save and reload account records using a local file, including recovery checks for invalid records and a temporary-file replacement approach for writes.
- **Input validation:** Validate usernames, passwords, and monetary amounts; store balances as 64-bit integer cents instead of floating-point values.
- **Automated verification:** Test core functionality with CTest and build-and-test checks in GitHub Actions.

## Tech stack

| Component | Technology |
| --- | --- |
| Language | C++17 |
| Cryptography | OpenSSL |
| Build system | CMake |
| Testing | CTest and GitHub Actions |
| Persistence | C++ file streams and a local record file |

## Architecture

The application separates the command-line interface, account operations, authentication utilities, validation, and persistence.

```text
Cpp-Banking-and-Authentication/
├── .github/workflows/ci.yml   # Automated build and tests
├── .gitignore
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp                # Command-line menus and application flow
│   ├── Account.h / .cpp        # Account state, balances, and authentication
│   ├── AccountManager.h / .cpp # Account collection and file persistence
│   ├── Crypto.h / .cpp         # OpenSSL hashing, salts, and hex conversion
│   └── InputHandling.h / .cpp  # Input checks and money parsing
└── tests/
    └── test_core.cpp           # Core functionality and regression tests
```

## Build and run

**Requirements:** CMake 3.16+, a C++17 compiler, and OpenSSL development libraries.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/secure_bank
```

Use the menu to create an account, log in, view the balance, deposit or withdraw funds, and update the account password. Account data is stored in `data/accounts.db` relative to the working directory.

## Implementation details

**Authentication:** Account credentials are represented by a random 16-byte salt and a 32-byte PBKDF2-HMAC-SHA256 hash using 600,000 iterations. Password hashes, rather than plaintext passwords, are saved to disk. The local file format does not store a per-account iteration count.

**Balances:** Monetary input is parsed into `std::int64_t` cents to avoid floating-point rounding in transaction calculations. Deposits and withdrawals check invalid amounts, available funds, and overflow.

**Persistence:** `AccountManager` stores accounts in a `std::vector<Account>`, loads validated records on startup, and writes updates through a temporary file before replacing the database file. Account operations attempt to restore their previous state if saving fails.

## Testing

The `tests/test_core.cpp` suite covers input validation, money parsing and boundary conditions, hashing, account creation and authentication, transactions, password changes, persistence, and malformed stored data. The GitHub Actions workflow builds the project, runs CTest, and checks that the application starts and exits.

## Scope and limitations

This is a **personal software project and local banking simulation**, not a deployed financial service or a production banking platform. Its file-based storage is neither encrypted nor tamper-resistant. The application does not implement login rate limiting, multi-factor authentication, password masking, concurrent access, or a transactional ledger. Do not use real financial data or passwords used for other services.
