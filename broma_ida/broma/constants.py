from typing import Literal

# no android32 nor android64 because they have symbols
BROMA_PLATFORMS = Literal["win", "mac", "ios"]
