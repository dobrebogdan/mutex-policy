add_library(mtxpol
        ../common/utils.cpp

        src/communication.cpp
        src/mtxpol.cpp
        src/random.cpp)
target_include_directories(mtxpol PUBLIC include)
target_include_directories(mtxpol PUBLIC ../common)
