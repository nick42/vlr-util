### Intent

The namespace macros are intended to allow declarative namespaces without triggering the automatic indentation which is prevelant in IDE code formatting.

### Suggested Usage

```C++
NAMESPACE_BEGIN( Foo )

// Stuff in Foo::
// (without every single line in the entire file being indented)

NAMESPACE_END //( Foo )
```
