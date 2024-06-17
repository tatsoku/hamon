#!/usr/bin/env bash

# exMeteo: clear_vgcores.sh
#
# Licensed under the:
# BSD 3-Clause License
#
# Copyright (c) 2024, H4rl
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
# Made with <3 by h4rl.

__NAME__="hShell: clear_vgcores.sh"
__AUTHOR__="h4rl"
__DESCRIPTION__="Clears any and all vgcores in the repo."
__LICENSE__="BSD 3-Clause License"
__VERSION__="0.1.0"

PWD=$(pwd)
COLOR=true

if ${COLOR}; then
	RED='\033[0;31m'
	GREEN='\033[0;32m'
	#YELLOW='\033[1;33m'
	#BLUE='\033[0;34m'
	CYAN='\033[0;36m'
	CLEAR='\033[0m'
fi

print_help() {
	echo -e "${RED}${__NAME__}${CLEAR} v${GREEN}${__VERSION__}${CLEAR}"
	echo -e "Licensed under: ${CYAN}${__LICENSE__}${CLEAR}\n"
	echo -e "USAGE: ${GREEN}${0}${CLEAR}\n"
	echo -e "Made with ${RED}<3${CLEAR} by ${__AUTHOR__}"
}

# Delete all vgcore.* in the repo.

delete() {
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

case "${1}" in
"-h" | "--help")
	print_help
	;;
*)
	delete
	;;
esac
