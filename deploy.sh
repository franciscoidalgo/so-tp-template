#!/bin/bash
UTNSO_LIBRARIES=("so-commons-library" "so-nivel-gui-library")
CWD=$PWD
MODULOS=("modulo1" "modulo2")
for UTNSO_LIB in ${UTNSO_LIBRARIES[@]}
do
    echo -e "\n\nInstalling ${UTNSO_LIB}\n\n"
    git clone "https://github.com/sisoputnfrba/${UTNSO_LIB}.git" #|| echo -e "\nLibrary is already cloned"
    cd $UTNSO_LIB
    sudo make uninstall
    sudo make all
    sudo make install
    cd $CWD
    sudo rm -f -r $UTNSO_LIB
    echo -e "\n\nDone!!!"
#    if ! cat .gitignore | grep -q "${UTNSO_LIB}/*"
#    then
#        echo -e "\nAdded ${UTNSO_LIB} to .gitignore"
#        echo -e "\n${UTNSO_LIB}/*" | cat >> .gitignore
#    fi
done

for MODULO in ${MODULOS[@]}
do
    echo -e "\n\nBuilding ${MODULO}\n\n"
    make release -C $MODULO
    echo -e "\n\nDone!!!\n\n"
done