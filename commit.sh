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

    cd utils
    git pull origin main

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
    echo "==== lilac/utils ===="
    echo ""

    cd ../core/utils

    git add --all
    git commit -a
    git push origin main

    echo ""
    echo "==== lilac/core ===="
    echo ""

    cd ..

    git add --all
    git commit -a
    git push origin main

fi
