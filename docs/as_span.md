### Intent

`as_span<T>` is intended as a parameter type to methods, which does automatic conversion from other types which can be interpreted as a span.

### Suggested Usage

This class is derived from the logical span type, and is intended to be used primarily in method parameters, where automatic conversion is desired. For example, a `std::string` can be interpreted as a span of BYTEs (and is often utilized to contain such). Rather than casting explicitly at the point of the call, `as_span<BYTE>` can be utilized to remove the extraneous cast, and simplify the usage at the point of call.

```c++
void foo( as_span<BYTE> spnData )
{
    // do stuff...
}

std::string sSomeData{ "blahblah" };
foo( sSomeData );

std::vector<BYTE> oDataArray{ 1, 2, 3 };
foo( oDataArray );
```

### Notes

The implementation has constructors for common types which are utilized as spans. Users are encouraged to further subclass the type, following the established style, if they have additional types which need to be interpreted as spans.

The current implementation can interpret collections of one element type as spans of another type, which can lead to mis-use. This is intended (ie: interpreting a collection of wchar_t as BYTEs), but potentially dangerous.
