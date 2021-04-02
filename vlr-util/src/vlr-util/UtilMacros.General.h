#pragma once

#define VLR_IF_NOT_NULL( pointer ) if (pointer == nullptr) {} else pointer
#define VLR_IF_NOT_NULL_DEREF( pointer ) if (pointer == nullptr) {} else (*pointer)
