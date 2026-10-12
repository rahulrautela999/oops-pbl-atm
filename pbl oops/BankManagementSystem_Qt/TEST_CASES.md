# Test Cases

| Test | Expected Result |
|---|---|
| Deposit 500 | Balance increases by 500 |
| Deposit 0 | Rejected |
| Deposit -100 | Rejected |
| Savings withdraw within minimum balance | Accepted |
| Savings withdraw causing balance below minimum | Rejected |
| Checking withdraw within overdraft | Accepted |
| Checking withdraw beyond overdraft | Rejected |
| Monthly savings update | Interest is added |
| Monthly checking update | Monthly fee is deducted |
| Create Savings Account | New SavingsAccount object created |
| Create Checking Account | New CheckingAccount object created |
| Monthly update on Account* | Correct derived function executes |

## Example

Savings:
- Balance = $8,450
- Interest rate = 5%
- Monthly interest = 8450 × 0.05 / 12
- Interest is added to balance.

Checking:
- Balance = $7,000
- Monthly fee = $10
- New balance = $6,990.

## Viva Demonstration

Show these four points:

1. `Account` cannot be instantiated because it contains pure virtual functions.
2. `SavingsAccount` and `CheckingAccount` inherit common behavior.
3. Both classes implement `withdraw()` differently.
4. `BankManager` calls `applyMonthlyUpdate()` through `Account*`, demonstrating runtime polymorphism.
