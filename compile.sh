#!/bin/bash

# PCANNON COMPILE.SH v1.3 - FROM PCANNON PROJECT STANDARDS
# STANDARD: 20260714
# https://github.com/pcannon09/pcannonProjectStandards

set -e

# ROOT CHECK

if [[ ! -f ".root_dir" ]]; then
	echo "Need a \`.root_dir\` file for indication"
	echo "Execute \`gen.sh\` at the repo root"
	exit 1
fi

if [[ ! -d "./build/bin/EFI/BOOT/" ]]; then
	mkdir -pv ./build/bin/EFI/BOOT/
fi

# IMPORTS

source "./utils/inc/sh/colors.sh"

# CONSTANTS

readonly COMPILATION_FILE_PATH=".private/dev/compilation.json"
readonly PROJECT_INFO_PATH=".private/project.json"
readonly BUILD_DIR="./build"
readonly BACKUP_DIR="./build/bin/exeBackup"

readonly DEFAULT_BUILD_TYPE="Debug"

export NINJA_STATUS="${BOLD}${BRIGHT_BLACK}[ ${GREEN}%p${BRIGHT_BLACK} :: ${RED}%u/%t${BRIGHT_BLACK} :: ${MAGENTA}%o/s${BRIGHT_BLACK} :: ${CYAN}%e${BRIGHT_BLACK} ] ${RESET}"

# GLOBAL STATE

CHECKS=true
BUILD_TYPE="$DEFAULT_BUILD_TYPE"

cores=""
enableBackup=""
projectName=""
compilerFlags=""

declare -a compileMacros=()
declare -a compileCMakeOptions=()
declare -a compileCMakeArguments=()
declare -a cmakeCommand=()

# UTILITY FUNCTIONS

__printError() {
	echo -e "${BRIGHT_RED}[ ERROR ] $*${RESET}"
}

__printInfo() {
	echo -e "${BRIGHT_BLUE}${BOLD}[ INFO ] $*${RESET}"
}

__printWarning() {
	echo -e "${BRIGHT_YELLOW}${BOLD}[ WARN ] $*${RESET}"
}

__commandExists() {
	command -v "$1" > /dev/null 2>&1
}

# ARGUMENT HANDLING

__parseArguments() {
	if [[ "$1" == "nochecks" || "$2" == "nochecks" || "$3" == "nochecks" ]]; then
		CHECKS=false
	fi

	if [[ "$2" == "ndev" ]]; then
		BUILD_TYPE="Release"
	fi
}

# DEPENDENCY CHECKS

__checkDependencies() {
	[[ "$CHECKS" == true ]] || return 0

	if ! __commandExists cmake; then
		__printError "Please have 'cmake' installed"
		exit 1
	fi

	if ! __commandExists ninja && ! __commandExists make; then
		__printError "Please have 'ninja' or 'make' installed"
		exit 1
	fi

	if ! __commandExists jq; then
		__printError "Please have 'jq' installed"
		exit 1
	fi
}

# CONFIGURATION

__loadConfiguration() {
	cores=$(jq '.cores' "$COMPILATION_FILE_PATH")
	enableBackup=$(jq -r '.enableBackup' "$COMPILATION_FILE_PATH")
	projectName=$(jq -r '.exeName' "$PROJECT_INFO_PATH")

	mapfile -t compileMacros < <(
		jq -r '.macros // [] | .[]' "$COMPILATION_FILE_PATH"
	)

	mapfile -t compileCMakeOptions < <(
		jq -r '.cmakeOptions // [] | .[]' "$COMPILATION_FILE_PATH"
	)

	mapfile -t compileCMakeArguments < <(
		jq -r '.cmakeArguments // [] | .[]' "$COMPILATION_FILE_PATH"
	)
}

# BUILD DIRECTORY

__ensureBuildDirectory() {
	mkdir -p "$BUILD_DIR"
}

# BACKUP

__backupExecutable() {
	case "$enableBackup" in
		YES|yes|y)
			;;
		*)
			return 0
			;;
	esac

	printf "${BOLD}${GREEN}[ INFO ] Backing up executable\n${RESET}"

	mkdir -p "$BACKUP_DIR"

	if [[ -f "$BUILD_DIR/bin/$projectName" ]]; then
		cp \
			"$BUILD_DIR/bin/$projectName" \
			"$BACKUP_DIR/$projectName-$(date +%s)"
	fi

	echo -e " [ Done ]"
}

# BUILD

__compileSoftware() {
	set -o pipefail

	if ! cmake --build "$BUILD_DIR" -j"$cores" -v; then
		exit 1
	fi

	# ./utils/exec/qemuBuild.sh
}

# CMAKE MACROS

__buildCompilerFlags() {
	compilerFlags=""

	for macro in "${compileMacros[@]}"; do
		if [[ -n "$macro" ]]; then
			compilerFlags+=" -D$macro"
		else
			__printWarning "Skipping empty macro \`$macro\`"
		fi
	done
}

# CMAKE OPTIONS

__appendCMakeOptions() {
	local option

	for option in "${compileCMakeOptions[@]}"; do
		if [[ -n "$option" ]]; then
			cmakeCommand+=("$option")
		else
			__printWarning "Skipping empty CMake option"
		fi
	done
}

__appendCMakeArguments() {
	local argument

	for argument in "${compileCMakeArguments[@]}"; do
		if [[ -n "$argument" ]]; then
			cmakeCommand+=("$argument")
		else
			__printWarning "Skipping empty CMake argument"
		fi
	done
}

# CMAKE SETUP

__setupCMake() {
	__buildCompilerFlags

	cmakeCommand=(
		cmake
		-S .
		-B "$BUILD_DIR"
		-G Ninja
		-DCMAKE_BUILD_TYPE="$BUILD_TYPE"
		-DCMAKE_POLICY_VERSION_MINIMUM=3.5
		-DCMAKE_C_FLAGS="$compilerFlags"
	)

	__appendCMakeOptions
	__appendCMakeArguments

	__printInfo "Running CMake:"
	printf ' %q' "${cmakeCommand[@]}"
	echo

	"${cmakeCommand[@]}"
}

# SETTINGS

__createCompilationConfig() {
	if [[ ! -s "$COMPILATION_FILE_PATH" ]]; then
		mkdir -p "$(dirname "$COMPILATION_FILE_PATH")"

		cat > "$COMPILATION_FILE_PATH" <<EOF
{
	"cores": 10,
	"enableBackup": "NO",
	"macros": [],
	"cmakeOptions": [],
	"cmakeArguments": []
}
EOF
	fi
}

__readCoreSetting() {
	echo -e "${BOLD}[ PROMPT ] Enter the number of cores to compile the program ('same' to keep current)${RESET}"
	read -r cores

	case "$cores" in
		same)
			cores=$(jq '.cores' "$COMPILATION_FILE_PATH")
			echo -e "${GREEN}$cores${RESET}"
			;;

		[0-9]*)
			if [[ ! "$cores" =~ ^[0-9]+$ ]]; then
				__printError "Give a number for \`cores\`, not a string or boolean"
				exit 1
			fi
			;;

		*)
			__printError "Give a number for \`cores\`, not a string or boolean"
			exit 1
			;;
	esac
}

__readBackupSetting() {
	echo -e "${BOLD}[ PROMPT ] Enable backup? 'YES' or 'NO' ('same' to keep current)${RESET}"
	read -r enableBackup

	if [[ "$enableBackup" == "same" ]]; then
		enableBackup=$(jq -r '.enableBackup' "$COMPILATION_FILE_PATH")
		echo -e "${GREEN}$enableBackup${RESET}"
	fi
}

__readMacrosSetting() {
	local macroVal

	declare -a programMacros=()

	echo -e "${BOLD}[ PROMPT ] Macros (OPTIONAL)
(done: Stop adding)
(clear: Clear list)
(same: Keep current list)${RESET}"

	while IFS= read -r macroVal; do
		case "$macroVal" in
			same)
				mapfile -t programMacros < <(
					jq -r '.macros // [] | .[]' "$COMPILATION_FILE_PATH"
				)

				echo "[ CURRENT MACROS ]"
				printf "${GREEN} - %s\n" "${programMacros[@]}"
				printf "${RESET}"

				break
				;;

			done)
				break
				;;

			clear)
				programMacros=()
				echo "[ CLEARED ]"
				;;

			"")
				__printWarning "Skipping empty macro"
				;;

			*)
				programMacros+=("$macroVal")
				;;
		esac
	done

	echo -e "${BRIGHT_BLUE}${BOLD}[ * ] TOTAL MACROS [${programMacros[*]}]${RESET}"

	PROGRAM_MACROS=("${programMacros[@]}")
}

__readCMakeOptionsSetting() {
	local optionVal

	declare -a programCMakeOptions=()

	echo -e "${BOLD}[ PROMPT ] CMake options (OPTIONAL)
(done: Stop adding)
(clear: Clear list)
(same: Keep current list)${RESET}"

	while IFS= read -r optionVal; do
		case "$optionVal" in
			same)
				mapfile -t programCMakeOptions < <(
					jq -r '.cmakeOptions // [] | .[]' "$COMPILATION_FILE_PATH"
				)

				echo "[ CURRENT CMAKE OPTIONS ]"
				printf "${GREEN} - %s\n" "${programCMakeOptions[@]}"
				printf "${RESET}"

				break
				;;

			done)
				break
				;;

			clear)
				programCMakeOptions=()
				echo "[ CLEARED ]"
				;;

			"")
				__printWarning "Skipping empty CMake option"
				;;

			*)
				programCMakeOptions+=("$optionVal")
				;;
		esac
	done

	echo -e "${BRIGHT_BLUE}${BOLD}[ * ] TOTAL CMAKE OPTIONS [${programCMakeOptions[*]}]${RESET}"

	PROGRAM_CMAKE_OPTIONS=("${programCMakeOptions[@]}")
}

__readCMakeArgumentsSetting() {
	local argumentVal

	declare -a programCMakeArguments=()

	echo -e "${BOLD}[ PROMPT ] CMake arguments (OPTIONAL)
(done: Stop adding)
(clear: Clear list)
(same: Keep current list)${RESET}"

	while IFS= read -r argumentVal; do
		case "$argumentVal" in
			same)
				mapfile -t programCMakeArguments < <(
					jq -r '.cmakeArguments // [] | .[]' "$COMPILATION_FILE_PATH"
				)

				echo "[ CURRENT CMAKE ARGUMENTS ]"
				printf "${GREEN} - %s\n" "${programCMakeArguments[@]}"
				printf "${RESET}"

				break
				;;

			done)
				break
				;;

			clear)
				programCMakeArguments=()
				echo "[ CLEARED ]"
				;;

			"")
				__printWarning "Skipping empty CMake argument"
				;;

			*)
				programCMakeArguments+=("$argumentVal")
				;;
		esac
	done

	echo -e "${BRIGHT_BLUE}${BOLD}[ * ] TOTAL CMAKE ARGUMENTS [${programCMakeArguments[*]}]${RESET}"

	PROGRAM_CMAKE_ARGUMENTS=("${programCMakeArguments[@]}")
}

__writeCompilationConfig() {
	local macrosJson
	local cmakeOptionsJson
	local cmakeArgumentsJson

	macrosJson=$(
		printf '%s\n' "${PROGRAM_MACROS[@]}" |
			jq -R . |
			jq -s .
	)

	cmakeOptionsJson=$(
		printf '%s\n' "${PROGRAM_CMAKE_OPTIONS[@]}" |
			jq -R . |
			jq -s .
	)

	cmakeArgumentsJson=$(
		printf '%s\n' "${PROGRAM_CMAKE_ARGUMENTS[@]}" |
			jq -R . |
			jq -s .
	)

	mkdir -p tmp

	jq \
		--argjson cores "$cores" \
		--arg enableBackup "$enableBackup" \
		--argjson macros "$macrosJson" \
		--argjson cmakeOptions "$cmakeOptionsJson" \
		--argjson cmakeArguments "$cmakeArgumentsJson" \
		'
		.cores = $cores
		| .enableBackup = $enableBackup
		| .macros = $macros
		| .cmakeOptions = $cmakeOptions
		| .cmakeArguments = $cmakeArguments
		' \
		"$COMPILATION_FILE_PATH" > tmp/tmp_dev_compilation.json

	mv \
		tmp/tmp_dev_compilation.json \
		"$COMPILATION_FILE_PATH"
}

__settings() {
	echo -e "[ * ] Compilation settings"

	mkdir -p tmp

	__createCompilationConfig
	__readCoreSetting
	__readBackupSetting
	__readMacrosSetting
	__readCMakeOptionsSetting
	__readCMakeArgumentsSetting
	__writeCompilationConfig

	echo -e "[ DONE ]"
}

# MAIN

__parseArguments "$@"
__checkDependencies
__createCompilationConfig
__loadConfiguration
__ensureBuildDirectory
__backupExecutable

case "$1" in
	setup)
		__setupCMake "$@"
		;;

	settings)
		__settings
		;;

	""|m|nochecks)
		__compileSoftware "$@"
		;;

	*)
		__printError "Unknown command: $1"
		exit 1
		;;
esac

