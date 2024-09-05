#!/usr/bin/env bash

# hamon!: build.sh
#
# built with this inspirational message in mind: Why tf would we use make
#
# Licensed under the:
# BSD 3-Clause License
#
# Copyright (c) 2024, tatsoku
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
# Made with <3 by tatsoku-org!

shopt -s nullglob

__NAME__="hamon!: build.sh"
__AUTHOR__="h4rl"
__DESCRIPTION__="Compiles & links hamon into an executable."
__LICENSE__="BSD 3-Clause License"
__VERSION__="0.1.0"

SRC="$(pwd)/src"
BUILD="$(pwd)/build"
OUT="${BUILD}/out"
BIN="${BUILD}/bin"
DIR="${SRC}/hamon"

TESTS="$(pwd)/tests"
TESTS_OUT="${BUILD}/tests/out"
TESTS_BIN="${BUILD}/tests/bin"

UNITY_TAG="v2.6.0"

#INCLUDE="$(pwd)/include"
COLOR=true

CC="clang"

if ${COLOR}; then
	ESCAPE=$(printf "\e")
	RED="${ESCAPE}[0;31m"
	GREEN="${ESCAPE}[0;32m"
	YELLOW="${ESCAPE}[1;33m"
	BLUE="${ESCAPE}[0;34m"
	CYAN="${ESCAPE}[0;36m"
	CLEAR="${ESCAPE}[0m"
fi

CFLAGS="-O3 -Wall -Wextra -I./include/"

LINKER_FLAGS="-Wall -Wextra"

if [[ ${OSTYPE} != "msys" ]]; then
	LINKER_FLAGS+=" -fuse-ld=mold"
else
	CFLAGS+=" -I/usr/include"
	LINKER_FLAGS="-lbsd ${LINKER_FLAGS}"
fi

if [[ ${2} == "--debug" ]]; then
	CFLAGS+=" -ggdb"
fi

if [[ ! -d ${OUT} ]]; then
	mkdir "${OUT}"
fi

handle_failure() {
	local MESSAGE="${1}"

	if [[ -z ${MESSAGE} ]]; then
		echo "No message provided, are you using this right?"
		exit 1
	fi

	echo "${RED}!${CLEAR} ${MESSAGE}${CLEAR}"
	exit 1
}

print_help() {
	cat <<EOF
${RED}${__NAME__}${CLEAR} v${GREEN}${__VERSION__}${CLEAR}
Licensed under: ${CYAN}${__LICENSE__}${CLEAR}

USAGE: ${GREEN}${0}${CLEAR} ${RED}[FLAGS] ${BLUE}[EXECUTABLE_NAME]${CLEAR}
${RED}-c${CLEAR} | ${RED}--compile${CLEAR}
Compiles ${CYAN}${SRC}/main.c${CLEAR} & all files in ${CYAN}${DIR}/${CLEAR}.

${RED}-l${CLEAR} | ${RED}--link${CLEAR}
Links all the files in ${CYAN}${OUT}/${CLEAR} to an executable with ${BLUE}[EXECUTABLE_NAME]${CLEAR}

${RED}-st${CLEAR} | ${RED}--setup-testing${CLEAR}
Sets up and installs unity in the correct path.
(Automatically runs if not already installed during --test)

${RED}-t${CLEAR} | ${RED}--test${CLEAR} [TEST_FILE]
Runs unit tests in ${TESTS}, you can also specify a ${BLUE}[TEST_FILE]${CLEAR}

${RED}-d${CLEAR} | ${RED}--delete${CLEAR} | ${RED}--clean${CLEAR}
Cleans all files in ${CYAN}${BIN}/${CLEAR} & ${CYAN}${OUT}/${CLEAR}

${RED}-vg${CLEAR} | ${RED}--delete-cores${CLEAR} | ${RED}--delete-vgcores${CLEAR} | ${RED}--clean-vgcores${CLEAR} | ${RED}--clean-cores${CLEAR}
Cleans all vgcore files generated by valgrind in the repo.

${RED}-h${CLEAR} | ${RED}--help${CLEAR}
Shows this message.

${RED}-f${CLEAR} | ${RED}--fix-clangd${CLEAR}
Generates a compile_commands.json for clangd.

Made with ${RED}<3${CLEAR} by ${__AUTHOR__}
EOF
}

# iterates over every file in src/exmeteo/ and compiles them, then compiles src/main.c, moving them to out/

compile() {
	local -a C_FILES

	local -a TRIMMED_C_FILES
	local -a TRIMMED_C_FILENAMES

	local RECOMPILE
	local TRIMMED_C_FILE
	local TRIMMED_C_FILENAME

	mapfile -t C_FILES < <(find "${DIR}" -type f -name "*.c")

	for ((i = 0; i < ${#C_FILES[@]}; i++)); do
		TRIMMED_C_FILE="${C_FILES[${i}]%.*}"
		TRIMMED_C_FILENAME="${TRIMMED_C_FILE##*/}"
		TRIMMED_C_FILES+=("${TRIMMED_C_FILE}")
		TRIMMED_C_FILENAMES+=("${TRIMMED_C_FILENAME}")
		echo -e "${BLUE}>${CLEAR} Compiling: ${CYAN}${C_FILES[${i}]}${CLEAR}"
		if [[ -f "${OUT}/${TRIMMED_C_FILENAME}.o" ]]; then
			echo -ne "${YELLOW}!${CLEAR} ${CYAN}${TRIMMED_C_FILENAME}.o${CLEAR} seems to already exist, you wanna recompile it? [${GREEN}Y${CLEAR}/${RED}n${CLEAR}]: "
			read -r RECOMPILE
			if [[ ! ${RECOMPILE} =~ [Nn] ]]; then
				# shellcheck disable=SC2086
				"${CC}" ${CFLAGS} -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
			fi
		else
			# shellcheck disable=SC2086
			"${CC}" ${CFLAGS} -c "${C_FILES[${i}]}" -o "${OUT}/${TRIMMED_C_FILENAME}.o"
		fi
	done

	echo -e "${BLUE}>${CLEAR} Compiling: ${CYAN}${SRC}/main.c${CLEAR}"
	# shellcheck disable=SC2086
	"${CC}" ${CFLAGS} -c "${SRC}/main.c" -o "${OUT}/main.o"

	for i in "${!TRIMMED_C_FILENAMES[@]}"; do TRIMMED_C_FILENAMES[i]="${TRIMMED_C_FILENAMES[i]}.c"; done
	echo -e "${GREEN}✓${CLEAR} Compiled ${CYAN}${TRIMMED_C_FILENAMES[*]}${CLEAR} & ${CYAN}main.c${CLEAR} successfully"
}

# links all object files in out/ to an executable in /bin

link() {
	local RELINK
	local -a OBJECTS
	local EXECUTABLE_NAME
	local -a TRIMMED_FILES

	if [[ ! -d ${BIN} ]]; then
		mkdir "${BIN}"
	fi

	if [[ -n ${1} ]]; then
		EXECUTABLE_NAME="${1}"
	else
		echo -e "${RED}!${CLEAR} No executable name set, using default."
		EXECUTABLE_NAME="hsh"
	fi

	mapfile -t OBJECTS < <(find "${OUT}" -type f -name "*.o")

	TRIMMED_FILES=("${OBJECTS[@]##*/}")
	pushd "${OUT}" >/dev/null || handle_failure "Failed to pushd" #|| echo "Failed to pushd" && exit 1

	echo -e "${BLUE}>${CLEAR} Linking: ${CYAN}${TRIMMED_FILES[*]}${CLEAR}"

	if [[ -f "${BIN}/${EXECUTABLE_NAME}" ]]; then
		echo -ne "${YELLOW}!${CLEAR} ${CYAN}${EXECUTABLE_NAME}${CLEAR} seems to already exist, you wanna relink it? [${GREEN}Y${CLEAR}/${RED}n${CLEAR}]: "
		read -r RELINK
		if [[ ! ${RELINK} =~ [Nn] ]]; then
			# shellcheck disable=SC2086,SC2048
			"${CC}" ${LINKER_FLAGS} ${CFLAGS} -o "${BIN}/${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]}
			echo -e "${GREEN}✓${CLEAR} Linked ${CYAN}${TRIMMED_FILES[*]}${CLEAR} to ${GREEN}${EXECUTABLE_NAME}${CLEAR} successfully"
		fi
	else
		# shellcheck disable=SC2086,SC2048
		"${CC}" ${LINKER_FLAGS} ${CFLAGS} -o "${BIN}/${EXECUTABLE_NAME}" ${TRIMMED_FILES[*]}
		echo -e "${GREEN}✓${CLEAR} Linked ${CYAN}${TRIMMED_FILES[*]}${CLEAR} to ${GREEN}${EXECUTABLE_NAME}${CLEAR} successfully"
	fi

	popd >/dev/null || handle_failure "Failed to popd" # || echo "Failed to popd" && exit 1
}

setup_unity() {
	mkdir "${TESTS}/unity"
	mkdir temp_dir

	pushd temp_dir >/dev/null || handle_failure "Failed to pushd"
	git clone --branch "${UNITY_TAG}" https://github.com/ThrowTheSwitch/Unity

	rm Unity/src/meson.build
	mv Unity/src/* "${TESTS}/unity"

	popd >/dev/null || handle_failure "Failed to popd"

	rm -fr temp_dir
}

unit_test() {
	local -a TEST_FILES
	local TEST_FILE
	local TRIMMED_TEST_FILE
	local TRIMMED_TEST_FILENAME

	local -a TESTBINS
	local TEST_BINARY
	local TRIMMED_BINARY
	local TRIMMED_BINARY_NAME

	local -a DEPS
	local -a DEPS_O
	local -a DEPS_C
	local -a DEPS_PATHS

	if [[ ! -d ${TESTS}/unity ]]; then
		setup_unity
	fi

	if [[ ! -d ${TESTS_OUT} ]]; then
		mkdir -p "${TESTS_OUT}"
	fi

	if [[ ! -d ${TESTS_OUT}/unity ]]; then
		mkdir -p "${TESTS_OUT}/unity"
	fi

	if [[ ! -d ${TESTS_OUT}/deps ]]; then
		mkdir -p "${TESTS_OUT}/deps"
	fi

	if [[ ! -d ${TESTS_BIN} ]]; then
		mkdir -p "${TESTS_BIN}"
	fi

	mapfile -t TEST_FILES < <(find "${TESTS}" -maxdepth 1 -type f -name "*.c")

	if [[ -n "${1}" ]]; then
		mapfile -t TEST_FILES < <(find "${TESTS}" -maxdepth 1 -type f -name "${1}.c")
	fi

	pushd "${TESTS}" >/dev/null || handle_failure "Failed to pushd" #|| echo "Failed to pushd" && exit 1

	echo -e "${BLUE}>${CLEAR} Compiling: ${CYAN}unity.c${CLEAR}"
	# shellcheck disable=SC2086
	"${CC}" ${CFLAGS} -c "${TESTS}/unity/unity.c" -o "${TESTS_OUT}/unity/unity.o"
	echo -e "${GREEN}✓${CLEAR} Successfully compiled ${CYAN}unity.c${CLEAR}"

	for TEST_FILE in "${TEST_FILES[@]}"; do
		if [[ -s "${TEST_FILE}" ]]; then
			TRIMMED_TEST_FILE="${TEST_FILE%.*}"
			TRIMMED_TEST_FILENAME="${TRIMMED_TEST_FILE##*/}"

			eval "$(sed -n '1s/^\/\/[[:space:]]*//p' "${TEST_FILE}")"
			echo -e "\n\n\nevaluated: ${DEPS[*]}\n\n\n"

			for i in "${!DEPS[@]}"; do
				DEPS_C[i]="${DEPS[i]}.c"
				DEPS_O[i]="${DEPS[i]}.o"
				DEPS_PATHS[i]="${TESTS_OUT}/deps/${DEPS[i]}.o"

				echo -e "${BLUE}>${CLEAR} Compiling dependency: ${CYAN}${DEPS[i]}.c${CLEAR}"
				# shellcheck disable=SC2086
				"${CC}" ${CFLAGS} -c "${DIR}/${DEPS_C[i]}" -o "${DEPS_PATHS[i]}"
			done

			echo -e "${GREEN}✓${CLEAR} Successfully compiled dependencies: ${CYAN}${DEPS_C[*]}${CLEAR}"

			echo -e "${BLUE}>${CLEAR} Compiling test: ${CYAN}${TRIMMED_TEST_FILENAME}.c${CLEAR}"
			# shellcheck disable=SC2086
			"${CC}" ${CFLAGS} -c "./${TRIMMED_TEST_FILENAME}.c" -o "${TESTS_OUT}/${TRIMMED_TEST_FILENAME}.o"
			echo -e "${GREEN}✓${CLEAR} Successfully compiled test: ${CYAN}${TRIMMED_TEST_FILENAME}.c${CLEAR}"

			pushd "${TESTS_OUT}" >/dev/null || handle_failure "Failed to pushd" #|| echo "Failed to pushd" && exit 1

			echo "${DEPS_O[*]}"
			echo "${DEPS_PATHS[*]}"

			echo -e "${BLUE}>${CLEAR} Linking ${CYAN}${TRIMMED_TEST_FILENAME}.o unity.o ${DEPS_O[*]}${CLEAR} to test: ${GREEN}${TRIMMED_TEST_FILENAME}${CLEAR}"
			# shellcheck disable=SC2086,SC2048
			"${CC}" ${LINKER_FLAGS} ${CFLAGS} -o "${TESTS_BIN}/${TRIMMED_TEST_FILENAME}" "${TESTS_OUT}/${TRIMMED_TEST_FILENAME}.o" ${DEPS_PATHS[*]} "${TESTS_OUT}/unity/unity.o"
			echo -e "${GREEN}✓${CLEAR} Successfully linked ${CYAN}${TRIMMED_TEST_FILENAME}.o unity.o ${DEPS_O[*]}${CLEAR} to test: ${GREEN}${TRIMMED_TEST_FILENAME}${CLEAR}"
			popd >/dev/null || handle_failure "Failed to popd"

			unset DEPS DEPS_C DEPS_O DEPS_PATHS
		fi
	done

	mapfile -t TESTBINS < <(find "${TESTS_BIN}" -maxdepth 1 -type f)

	pushd "${TESTS_BIN}" >/dev/null || handle_failure "Failed to pushd" #|| echo "Failed to pushd" && exit 1

	for TEST_BINARY in "${TESTBINS[@]}"; do
		TRIMMED_BINARY="${TEST_BINARY%.*}"
		TRIMMED_BINARY_NAME="${TRIMMED_BINARY##*/}"

		echo -e "${BLUE}>${CLEAR} Running test: ${CYAN}${TRIMMED_BINARY_NAME}${CLEAR}"

		if [[ -n "${2}" && "${2}" == "--debug" ]]; then
			valgrind "./${TRIMMED_BINARY_NAME}"
		else
			"./${TRIMMED_BINARY_NAME}"
		fi
	done

	popd >/dev/null || handle_failure "Failed to popd" # || echo "Failed to popd" && exit 1

	#rm -fr "${TESTS_OUT}" "${TESTS_BIN}"
}

# removes dangling object files that shouldn't be there, used to be required, not that much as of lately though.

clean_dangling() {
	local DIR1
	local DIR2
	local LINE

	DIR1=${1}
	DIR2=${2}

	# Extract .c and .o filenames without paths and store them in temporary files
	find "$DIR1" -name "*.c" -exec basename {} \; >"temp_dir1_files.txt"
	find "$DIR2" -name "*.o" -exec basename {} \; >"temp_dir2_files.txt"
	echo "main.c" >>"temp_dir1_files.txt"

	# Compare the lists and find .o files in dir2 that do not have a corresponding .c file in dir1
	grep -Fxv -f <(sed 's/\.c$/.o/' "temp_dir1_files.txt") "temp_dir2_files.txt" >"extra_o_files.txt"

	# Remove extra .o files from dir2
	while read -r LINE; do
		rm -f "${DIR2}/${LINE}"
	done <"extra_o_files.txt"

	# Cleanup
	rm temp_dir1_files.txt temp_dir2_files.txt extra_o_files.txt
}

# cleans both /out && /bin

clean() {
	local CLEAN
	local LOCALOUT
	local LOCALBIN
	local LOG

	LOCALOUT=${1}
	LOCALBIN=${2}
	CONFIRMATION=${3}
	LOG=${4:true}

	if ${LOG}; then
		echo -e "${RED}!${CLEAR} Cleaning ${CYAN}${LOCALOUT}${CLEAR} & ${CYAN}${LOCALBIN}${CLEAR}."
	fi
	if ! [[ ${CONFIRMATION} =~ [yY] ]]; then
		echo -ne "${RED}!${CLEAR} You sure you want to proceed? [${GREEN}y${CLEAR}/${RED}N${CLEAR}]: "
		read -r CLEAN
	else
		CLEAN="y"
	fi
	if [[ ${CLEAN} =~ [Yy] ]]; then
		rm -fr "${LOCALOUT:?}/*"
		rm -fr "${LOCALBIN:?}/*"
		if ${LOG}; then
			echo -e "${GREEN}✓${CLEAR} Cleaned ${CYAN}${LOCALOUT}${CLEAR} & ${CYAN}${LOCALBIN}${CLEAR} successfully."
		fi
	else
		echo -e "${GREEN}✓${CLEAR} Cancelled."
	fi
}

clear_vgcores() {
	local -a FILES
	local FILE

	FILES=$(find "${PWD}" -type f -name "vgcore.*")

	if [[ -z ${FILES} ]]; then
		echo -e "${RED}!${CLEAR} No files found :("
		return
	fi

	for FILE in ${FILES}; do
		if [[ -f ${FILE} ]]; then
			rm "${FILE}"
			echo -e "${GREEN}✓${CLEAR} Deleted file ${CYAN}\"${FILE}\"${CLEAR}"
		else
			echo -e "${RED}!${CLEAR} File does not exist :("
		fi
	done
}

fix_clangd() {
	cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null
	mv build/compile_commands.json .
	find build -mindepth 1 ! -path "build/bin" ! -path "build/out" ! -path "build/tests" ! -name "build.sh" -exec rm -rf {} +
}

case $1 in
"-c" | "--compile")
	compile
	;;
"-l" | "--link")
	clean_dangling "${DIR}" "${OUT}"
	link "${2}"
	;;
"-st" | "--setup-testing")
	setup_unity
	;;
"-t" | "--test")
	unit_test "${2}" "${3}"
	;;
"-d" | "--delete" | "--clean")
	clean "${OUT}" "${BIN}" "n"
	;;
"-vg" | "--delete-cores" | "--delete-vgcores" | "--clean-vgcores" | "--clean-cores")
	clear_vgcores
	;;
"-f" | "--fix-clangd")
	fix_clangd
	;;
"--help" | "-h" | *)
	print_help "${@}"
	;;
esac