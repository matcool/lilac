@echo off

if "%1"=="pull" (

    git pull origin main

    cd submodules/lib
    git pull origin main

    cd submodules/core
    git pull origin main

) else (

    cd submodules/lib/submodules/core

    echo ""
    echo "==== lilac/core ===="
    echo ""

    git add --all
    git commit -a
    git push origin main

    echo ""
    echo "==== lilac/lib ===="
    echo ""

    cd ../..

    git add --all
    git commit -a
    git push origin main

    echo "==== lilac/loader ===="
    echo ""

    cd ../..

    git add --all
    git commit -a
    git push origin main

)
