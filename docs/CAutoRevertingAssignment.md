### Intent

`CAutoRevertingAssignment<T>` can be used to assign a value to a variable, which automatically reverts to the previous value when the object goes out of scope. In essence, this allows for a scoped assignment to a variable. This is somewhat equivalent, for example, to using a std::scoped_ptr with a custom cleanup object, but with a more straightforward semantic syntax.

#### Suggested Naming Convention

For coding standards with type indicators in variable names, the suggested prefix for this type is 'o' (ie: object). For multiple objects of this type in the same scope, incorporating the name of the affected variable into the name of the auto reverting instance is suggested (eg: `auto oAutoRevertingAssignment_Foo = ...`).

### Suggested Usage

There is a helper method to automatically capture the applicable types and instantiate the template correctly, without needing to explicitly specify such. 
```c++
int nValue = 0;
{
	auto oAutoRevert = vlr::MakeAutoRevertingAssignment( nValue, 42 );
	// The value of nValue here is 42
}
// The value of nValue here is 0
```

### Notes

The class can be instantiated with any value which can be converted to the type of the variable being assigned.

The object instance holds a reference to the variable being assigned to, so be cautious that its scope does not exceed the scope of the variable being assigned to.
