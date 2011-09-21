# Pull a stable branch + patches
BRANCH=7.10

rm -rf mesa
git clone git://anongit.freedesktop.org/git/mesa/mesa
# package the source archive and clean up:
( cd mesa 
  git checkout $BRANCH || exit 1
)
HEADISAT="$(cat mesa/.git/packed-refs | grep "refs/remotes/origin/${BRANCH}$" | cut -b1-7)"
# Cleanup.  We're not packing up the whole git repo.
( cd mesa && find . -type d -name ".git*" -exec rm -rf {} \; 2> /dev/null )
DATE=$(date +%Y%m%d)
mv mesa Mesa-${BRANCH}_${HEADISAT}
tar cf MesaLib-${BRANCH}_${HEADISAT}.tar Mesa-${BRANCH}_${HEADISAT}
xz -9 MesaLib-${BRANCH}_${HEADISAT}.tar
rm -rf MesaLib-${BRANCH}_${HEADISAT}
echo
echo "Mesa branch $BRANCH with HEAD at $HEADISAT packaged as MesaLib-${BRANCH}_${HEADISAT}.tar.xz"
echo
