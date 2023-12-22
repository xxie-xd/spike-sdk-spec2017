#!/bin/sh

# set -x

# subrepo_fetch_loop(repo, extra update option)
subrepo_shallow_fetch_loop() {
depth=10
until git submodule update --init ${2} --depth $depth --progress ${1}
do
	echo "[e] Fail to shallow fetch submodule ${2}, retry..."
done
}

# shallow_fetch_repo(parent directory, repo list, extra update option)
shallow_fetch_repo() {

echo "[*] Update repositories under ${1}"
cd ${1}

for repo in ${2}
do
	commit=$(git submodule status | grep -oe "\([0-9a-z]*\) $repo" | grep -oe "^\([0-9a-z]*\)")
	echo "[-] shallow fetch $repo -> $commit"
	subrepo_shallow_fetch_loop $repo "${3}"
done

cd $root
}

# subrepo_fetch_loop(repo, extra update option)
subrepo_full_fetch_loop() {
depth=10
until git submodule update --init ${2} --recursive --progress ${1}
do
	echo "[e] Fail to shallow fetch submodule ${2}, retry..."
done
}

# full_fetch_repo(parent directory, repo list, extra update option)
full_fetch_repo() {

echo "[*] Update repositories under ${1}"
cd ${1}

for repo in ${2}
do
	commit=$(git submodule status | grep -oe "\([0-9a-z]*\) $repo" | grep -oe "^\([0-9a-z]*\)")
	echo "[-] fully fetch $repo -> $commit"
	subrepo_full_fetch_loop $repo "${3}"
done

cd $root
}

root="$(dirname "$(readlink -f "$0")")"
NJOB=4

shallow_repo_list=${REPO_LIST:-"buildroot linux"}
full_repo_list=${REPO_LIST:-"riscv-pk"}


shallow_fetch_repo "$root/repo" "$shallow_repo_list" "--jobs $NJOB"
full_fetch_repo "$root/repo" "$full_repo_list" "--jobs $NJOB"

#toolchain_repo_list=${TOOLCHAIN_LIST:-"binutils gcc glibc newlib gdb"}
#shallow_fetch_repo "$root/repo/riscv-gnu-toolchain" "$toolchain_repo_list" "--recursive --jobs $NJOB"

echo "[*] done"


