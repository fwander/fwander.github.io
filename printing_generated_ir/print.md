# Print Pattern
# Context
This Pattern requires:
* Context Pattern

# Solution
```c
std::string s;
llvm::raw_string_ostream os {s};
module->print(os, nullptr);
std::cout << os.str() << std::endl;
```
# Example
# Variations
# Related patterns
# References