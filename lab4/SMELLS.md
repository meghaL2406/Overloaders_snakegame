# Test Smells — Reference List

Common test smells referenced in Lab 4 Part F.

## 1. Obscure Test
A test so long or complicated that its intent is unclear. No one can tell, by reading it, what behaviour it asserts.

## 2. Eager Test
One test method verifies more than one behaviour. When it fails you cannot tell which rule broke.

## 3. Mystery Guest
A test silently reads from or writes to a resource (file, database, global) that is not visible in the test body. You cannot understand the test without tracking down the external dependency.

## 4. General Fixture
A test shares a complex, one-size-fits-all setup with other tests. The fixture contains far more state than any single test needs, so the relationship between setup and assertion is obscured.

## 5. Assertion Roulette
Multiple assertions in one test, none of them labelled. When one fails the error message gives no clue which assertion fired.

## 6. Slow Test
A test takes so long (seconds, not milliseconds) that developers stop running the suite. Usually caused by real I/O (sleep, network, disk) or calling the full production binary.

## 7. Dead Code
Commented-out or unreachable test code left in the file. Looks like tested behaviour but verifies nothing.

## 8. Fragile Test
A test that breaks whenever unrelated production code changes — usually because it reaches into implementation details rather than observable behaviour.

## 9. Conditional Test Logic
An `if`/`switch` inside a test. Tests should be straight-line; branching hides the case that is never exercised.

## 10. Over-specified Test (Mock Abuse)
A test that checks every call and argument order on a mock rather than the observable outcome. Breaks on any refactoring even if behaviour is preserved.
