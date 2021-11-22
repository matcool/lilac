# use this file to automatically commit
# & push / pull all local changes to 
# loader and submodules

if [ $1 == "pull" ]; then

    git pull origin main

    cd submodules/lib
    git pull origin main
    cd ../..

    cd submodules/core
    git pull origin main
    cd ../..

    cd submodules/meta
    git pull origin master

else

    echo "==== lilac/loader ===="
    echo ""

    git add --all
    git commit -a
    git push origin main

    cd submodules

    echo ""
    echo "==== lilac/lib ===="
    echo ""

    cd lib

    git add --all
    git commit -a
    git push origin main

    echo ""
    echo "==== lilac/core ===="
    echo ""

    cd ../core

    git add --all
    git commit -a
    git push origin main

    echo ""
    echo "==== lilac/meta ===="
    echo ""

    cd ../meta

    git add --all
    git commit -a
    git push origin master

fi
