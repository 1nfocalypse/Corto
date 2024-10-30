# CONTRIBUTING

Thank you for wanting to contribute to Corto! If you don't already have an idea in mind, some potential targets for additions are
- Adding additional block cipher modes
- Restructuring the ciphers to utilize polymorphism for better type determination
- Restructuring command processing to utilize a command pattern
- Algorithmic optimization (i.e., reducing asymptotic runtime, not utilizing hardware acceleration)

If you already have a variation (or a bug fix), please feel free to open a pull request with the following information.
- Problem: What problem is your PR addressing?
- Solution: What is your solution to the problem?
- Implementation: How did you implement your solution?
- Proof of testing: Prove you ran and successfully passed the provided test cases by running the --verify argument. Additionally, if you are modifying the tool itself, please provide proof of functionality by showcasing correct nontrivial encryption and decryption in all modes along with a nontrivial 256 and 512 bit digest from Streebog.

Tests will be run on my machine as well. Please avoid inducing OS-specific dependencies in the program. Additionally, this is by nature a software-only implementation. While hardware will obviously speed up the process, the goal is for it to be relatively legible. Hardware will greatly obfuscate the functionality of the algorithms.
If you are merged, congratulations! Thank you for taking the time to make Corto better. If not, don't worry! I will do my best to provide specific, actionable feedback where appropriate. Good luck!
