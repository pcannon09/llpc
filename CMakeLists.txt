cmake_minimum_required(VERSION 3.24)

include(CMakePackageConfigHelpers)

# Project metadata

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.private/project.json" projectInfo)

string(JSON projectName GET "${projectInfo}" exeName)
string(JSON projectVersion GET "${projectInfo}" version)
string(JSON projectPrefix GET "${projectInfo}" projectPrefix)
string(JSON projectVersionState GET "${projectInfo}" versionState)
string(JSON projectVersionSTD GET "${projectInfo}" versionSTD)

project(${projectName}
	VERSION ${projectVersion}
	LANGUAGES C ASM_NASM
)

# Output directories

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Compiler configuration

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_C_VISIBILITY_PRESET hidden)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NASM_FORMAT elf64)

# Build options

option(LLPC_BUILD_FMT "Build LLPC FMT module" ON)
option(LLPC_BUILD_IO "Build LLPC IO module" ON)
option(LLPC_BUILD_LOGGING "Build LLPC Logging module" ON)
option(LLPC_BUILD_TIME "Build LLPC Time module" ON)
option(LLPC_BUILD_STRING "Build LLPC String module" ON)
option(LLPC_BUILD_ALLOC "Build LLPC Allocator module" ON)
option(LLPC_BUILD_UEFI "Build LLPC UEFI application" ON)

option(LLPC_BUILD_TEST "Build LLPC host test executable" OFF)
option(LLPC_ENABLE_SANITIZER "Enable sanitizers for host builds" OFF)

# Host platform

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(LLPC_HOST_PLATFORM WINDOWS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(LLPC_HOST_PLATFORM LINUX)
else()
	set(LLPC_HOST_PLATFORM "${UNKNOWN}-${CMAKE_SYSTEM_NAME}")
endif()

# Project paths

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc/${projectName}")
set(INC_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc")
set(UEFI_DIR "${SRC_DIR}/platform/uefi")

# Sources

set(LLPC_HEADERS
	${INC_DIR}/predefines.h
)

set(LLPC_SOURCES
	${SRC_DIR}/lib/globals.c
)

set(LLPC_LOGGING_SOURCES
	${SRC_DIR}/lib/logging/logger.c
)

set(LLPC_TIME_SOURCES
	${SRC_DIR}/lib/time/time.c
	${SRC_DIR}/lib/time/date.c
)

set(LLPC_IO_SOURCES
	${SRC_DIR}/lib/io/input.c
	${SRC_DIR}/lib/io/output.c
	${SRC_DIR}/lib/io/color.c
)

set(LLPC_STRING_SOURCES
	${SRC_DIR}/lib/string/string.c
)

set(LLPC_FMT_SOURCES
	${SRC_DIR}/lib/fmt/fmtConvert.c
)

set(LLPC_ALLOC_SOURCES
	${SRC_DIR}/lib/alloc/alloc_impl.c
	${SRC_DIR}/lib/alloc/calloc.c
	${SRC_DIR}/lib/alloc/free.c
	${SRC_DIR}/lib/alloc/malloc.c
	${SRC_DIR}/lib/alloc/realloc.c
)

set(LLPC_ASM_SOURCES)

# Common library compiler options

set(LLPC_COMMON_COMPILE_OPTIONS
	-fno-strict-aliasing
	-fno-stack-protector
	-fno-builtin
	-mno-red-zone

	-fshort-wchar

	-Wall
	-Wextra
	-Wpedantic
	-Wconversion
	-Wshadow
	-Wstrict-prototypes
	-Wno-pedantic
)

# UEFI application compiler options

set(LLPC_UEFI_COMPILE_OPTIONS
	-ffreestanding
	-fshort-wchar
	-fno-stack-protector
	-fno-stack-check
	-fno-strict-aliasing
	-fno-merge-constants
	-fno-builtin
	-mno-red-zone
	-mno-sse
	-mno-mmx
	-mno-sse2
	-fno-asynchronous-unwind-tables
	-fno-unwind-tables

	-Wall
	-Wextra
	-Wconversion
	-Wshadow
	-Wstrict-prototypes
)

set(LLPC_UEFI_COMPILE_DEFINITIONS
	GNU_EFI_USE_MS_ABI
)

# Host library helper

function(llpc_configure_library target)
	target_include_directories(${target}
		PUBLIC ${INC_ROOT_DIR}
		PRIVATE ${SRC_DIR}
	)

	target_compile_options(${target}
		PRIVATE
			${LLPC_COMMON_COMPILE_OPTIONS}
	)

	if(LLPC_ENABLE_SANITIZER)
		target_compile_options(${target}
			PRIVATE
				-fsanitize=address,undefined
				-fno-omit-frame-pointer
		)
	endif()
endfunction()

# Core library

add_library(${projectName} STATIC
	${LLPC_SOURCES}
)

llpc_configure_library(${projectName})

# String library

if(LLPC_BUILD_STRING)
	add_library(${projectName}_string STATIC
		${LLPC_STRING_SOURCES}
	)

	llpc_configure_library(${projectName}_string)
endif()

# FMT library

if(LLPC_BUILD_FMT)
	add_library(${projectName}_fmt STATIC
		${LLPC_FMT_SOURCES}
	)

	llpc_configure_library(${projectName}_fmt)
endif()

# Time library

if(LLPC_BUILD_TIME)
	add_library(${projectName}_time STATIC
		${LLPC_TIME_SOURCES}
	)

	llpc_configure_library(${projectName}_time)
endif()

# Logging library

if(LLPC_BUILD_LOGGING)
	add_library(${projectName}_logging STATIC
		${LLPC_LOGGING_SOURCES}
	)

	llpc_configure_library(${projectName}_logging)

	if(LLPC_BUILD_STRING)
		target_link_libraries(${projectName}_logging
			PUBLIC
				${projectName}_string
		)
	endif()
endif()

# IO library

if(LLPC_BUILD_IO)
	add_library(${projectName}_io STATIC
		${LLPC_IO_SOURCES}
	)

	llpc_configure_library(${projectName}_io)

	if(LLPC_BUILD_STRING)
		target_link_libraries(${projectName}_io
			PUBLIC
				${projectName}_string
		)
	endif()
endif()

# Allocator library

if(LLPC_BUILD_ALLOC)
	add_library(${projectName}_alloc STATIC
		${LLPC_ALLOC_SOURCES}
	)

	llpc_configure_library(${projectName}_alloc)
endif()

# Host test executable

if(LLPC_BUILD_TEST)
	enable_testing()

	add_executable(${projectName}_test
		${SRC_DIR}/test/main.c
	)

	llpc_configure_library(${projectName}_test)

	target_link_libraries(${projectName}_test
		PRIVATE
			${projectName}
	)

	if(LLPC_BUILD_STRING)
		target_link_libraries(${projectName}_test
			PRIVATE
				${projectName}_string
		)
	endif()

	if(LLPC_BUILD_FMT)
		target_link_libraries(${projectName}_test
			PRIVATE
				${projectName}_fmt
		)
	endif()

	if(LLPC_BUILD_IO)
		target_link_libraries(${projectName}_test
		PRIVATE
			${projectName}_io
		)
	endif()

	if(LLPC_BUILD_ALLOC)
		target_link_libraries(${projectName}_test
			PRIVATE
				${projectName}_alloc
		)
	endif()

	if(LLPC_ENABLE_SANITIZER)
		target_link_options(${projectName}_test
			PRIVATE
				-fsanitize=address,undefined
		)
	endif()
endif()

# UEFI application

if(LLPC_BUILD_UEFI)
	find_program(OBJCOPY objcopy REQUIRED)
	find_program(LD ld REQUIRED)
	find_program(NASM nasm REQUIRED)

	set(UEFI_CRT
		/usr/lib/crt0-efi-x86_64.o
	)

	set(UEFI_LINKER_SCRIPT
		/usr/lib/elf_x86_64_efi.lds
	)

	set(UEFI_GNUEFI
		/usr/lib/libgnuefi.a
	)

	set(UEFI_LIBEFI
		/usr/lib/libefi.a
	)

	if(NOT EXISTS "${UEFI_CRT}")
		message(FATAL_ERROR
			"GNU-EFI crt0 not found: ${UEFI_CRT}"
		)
	endif()

	if(NOT EXISTS "${UEFI_LINKER_SCRIPT}")
		message(FATAL_ERROR
			"GNU-EFI linker script not found: ${UEFI_LINKER_SCRIPT}"
		)
	endif()

	if(NOT EXISTS "${UEFI_GNUEFI}")
		message(FATAL_ERROR
			"GNU-EFI library not found: ${UEFI_GNUEFI}"
		)
	endif()

	if(NOT EXISTS "${UEFI_LIBEFI}")
		message(FATAL_ERROR
			"GNU-EFI library not found: ${UEFI_LIBEFI}"
		)
	endif()

	set(LLPC_UEFI_BUILD_DIR
		"${CMAKE_BINARY_DIR}/uefi"
	)

	set(LLPC_UEFI_OBJECT_DIR
		"${LLPC_UEFI_BUILD_DIR}/obj"
	)

	set(LLPC_UEFI_OUTPUT_DIR
		"${CMAKE_BINARY_DIR}/bin/EFI/BOOT"
	)

	set(LLPC_UEFI_SO
		"${LLPC_UEFI_BUILD_DIR}/${projectName}.so"
	)

	set(LLPC_UEFI_OUTPUT
		"${LLPC_UEFI_OUTPUT_DIR}/BOOTX64.EFI"
	)

	add_library(${projectName}_uefi_main OBJECT
		${SRC_DIR}/main.c
	)

	set_target_properties(${projectName}_uefi_main PROPERTIES
		POSITION_INDEPENDENT_CODE OFF
	)

	target_include_directories(${projectName}_uefi_main
		PRIVATE
			${INC_ROOT_DIR}
			${SRC_DIR}
	)

	target_compile_options(${projectName}_uefi_main
		PRIVATE
			${LLPC_UEFI_COMPILE_OPTIONS}
	)

	target_compile_definitions(${projectName}_uefi_main
		PRIVATE
			${LLPC_UEFI_COMPILE_DEFINITIONS}
	)

	set(LLPC_UEFI_SOURCES
		${LLPC_SOURCES}
	)

	if(LLPC_BUILD_STRING)
		list(APPEND LLPC_UEFI_SOURCES
			${LLPC_STRING_SOURCES}
		)
	endif()

	if(LLPC_BUILD_FMT)
		list(APPEND LLPC_UEFI_SOURCES
			${LLPC_FMT_SOURCES}
		)
	endif()

	if(LLPC_BUILD_IO)
		list(APPEND LLPC_UEFI_SOURCES
			${LLPC_IO_SOURCES}
		)
	endif()

	if(LLPC_BUILD_ALLOC)
		list(APPEND LLPC_UEFI_SOURCES
			${LLPC_ALLOC_SOURCES}
		)
	endif()

	if(LLPC_BUILD_TIME)
		list(APPEND LLPC_UEFI_SOURCES
			${LLPC_TIME_SOURCES}
		)
	endif()

	if(LLPC_BUILD_LOGGING)
		list(APPEND LLPC_UEFI_SOURCES
			${LLPC_LOGGING_SOURCES}
		)
	endif()

	add_library(${projectName}_uefi_libs OBJECT
		${LLPC_UEFI_SOURCES}
	)

	set_target_properties(${projectName}_uefi_libs PROPERTIES
		POSITION_INDEPENDENT_CODE OFF
	)

	target_include_directories(${projectName}_uefi_libs
		PRIVATE
			${INC_ROOT_DIR}
			${SRC_DIR}
	)

	target_compile_options(${projectName}_uefi_libs
		PRIVATE
			${LLPC_UEFI_COMPILE_OPTIONS}
	)

	target_compile_definitions(${projectName}_uefi_libs
		PRIVATE
			${LLPC_UEFI_COMPILE_DEFINITIONS}
	)

	set(LLPC_ASM_OBJECTS)

	foreach(ASM_SOURCE IN LISTS LLPC_ASM_SOURCES)
		get_filename_component(ASM_NAME
			"${ASM_SOURCE}"
			NAME_WE
		)

		set(ASM_OBJECT
			"${LLPC_UEFI_OBJECT_DIR}/${ASM_NAME}.o"
		)

		add_custom_command(
			OUTPUT "${ASM_OBJECT}"

			COMMAND ${CMAKE_COMMAND} -E make_directory
				"${LLPC_UEFI_OBJECT_DIR}"

			COMMAND ${NASM}
				-f ${NASM_FORMAT}
				"${ASM_SOURCE}"
				-o "${ASM_OBJECT}"

			DEPENDS
				"${ASM_SOURCE}"

			VERBATIM
		)

		list(APPEND LLPC_ASM_OBJECTS
			"${ASM_OBJECT}"
		)
	endforeach()

	add_custom_target(${projectName}_uefi_asm
		DEPENDS
			${LLPC_ASM_OBJECTS}
	)

	set(LLPC_UEFI_MAIN_OBJECTS
		$<TARGET_OBJECTS:${projectName}_uefi_main>
	)

	set(LLPC_UEFI_LIB_OBJECTS
		$<TARGET_OBJECTS:${projectName}_uefi_libs>
	)

	add_custom_command(
		OUTPUT "${LLPC_UEFI_SO}"

		COMMAND ${CMAKE_COMMAND} -E make_directory
			"${LLPC_UEFI_BUILD_DIR}"

		COMMAND ${LD}
			-nostdlib
			-shared
			-Bsymbolic
			-z nocombreloc
			-T "${UEFI_LINKER_SCRIPT}"
			"${UEFI_CRT}"
			${LLPC_UEFI_MAIN_OBJECTS}
			${LLPC_UEFI_LIB_OBJECTS}
			${LLPC_ASM_OBJECTS}
			--start-group
			"${UEFI_GNUEFI}"
			"${UEFI_LIBEFI}"
			--end-group
			-o "${LLPC_UEFI_SO}"

		COMMAND_EXPAND_LISTS

		DEPENDS
			${LLPC_UEFI_MAIN_OBJECTS}
			${LLPC_UEFI_LIB_OBJECTS}
			${LLPC_ASM_OBJECTS}
			"${UEFI_CRT}"
			"${UEFI_LINKER_SCRIPT}"
			"${UEFI_GNUEFI}"
			"${UEFI_LIBEFI}"

		VERBATIM
	)

	add_custom_command(
		OUTPUT "${LLPC_UEFI_OUTPUT}"

		COMMAND ${CMAKE_COMMAND} -E make_directory
			"${LLPC_UEFI_OUTPUT_DIR}"

		COMMAND ${OBJCOPY}
			-j .text
			-j .sdata
			-j .data
			-j .rodata
			-j .dynamic
			-j .dynsym
			-j .rela
			-j .reloc
			-O efi-app-x86_64
			"${LLPC_UEFI_SO}"
			"${LLPC_UEFI_OUTPUT}"

		DEPENDS
			"${LLPC_UEFI_SO}"

		VERBATIM
	)

	add_custom_target(${projectName}_uefi
		ALL
		DEPENDS
			"${LLPC_UEFI_OUTPUT}"
	)

	add_dependencies(
		${projectName}_uefi
		${projectName}_uefi_asm
	)

	message(STATUS "LLPC host platform: ${LLPC_HOST_PLATFORM}")
	message(STATUS "LLPC UEFI output: ${LLPC_UEFI_OUTPUT}")
endif()
