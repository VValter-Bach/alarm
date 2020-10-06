#!/bin/bash

R="\033[0;31m"
Y="\033[1;33m"
G="\033[0;32m"
NC="\033[0m"

variables () {

    echo -e "${Y}Getting and Setting the varaibles${NC}"

    #reading compiler settings
    echo -e "${Y}Choose Compiler$NC"
    echo -e "1 \t gcc"
    echo -e "2 \t gpp"
    echo -e "3 \t clang"
    echo -e "4 \t clang++"
    read number

    if [[ $number == 1 ]];then 
        cc="gcc"
        end="c"
    elif [[ $number == 2 ]];then 
        cc="gpp"
        end="cpp"
    elif [[ $number == 3 ]];then 
        cc="clang"
        end="c"
    elif [[ $number == 4 ]];then 
        cc="clang++"
        end="cpp"
    fi

    #setting flags and others
    flags="-Wall -Wextra -O3"
    linker="-lpthread -lwiringPi -lm"

    #find all source file and generate object filenames
    src=($(find . -name \*.$end))
    src_files=($(find . -name \*.${end} -printf "%f\n"))

    for((i=0; i<${#src[*]}; i++))
    do
        src[$i]="${src[$i]/\.\//}"
        clang-format -i --style="WebKit" "${src[$i]}"
    done

    obj=()
    for item in ${src_files[*]}
    do
        obj+=("obj/${item/\.c/.o}")
    done

    #finding executable name
    test=($(find . -type l))
    len=${#test[@]}

    if [[ $len == 1 ]]; then
        echo -e "${G}Project Link found.${NC}"
        out=${test/\.\//}
    else
        echo -e "${Y}Please enter the name of the executable${NC}"
        read out
    fi
}

requirements () {

    echo -e "${Y}Checking Requirements${NC}"
    if test -d obj; then
    	echo -e "${G}Directrory obj/ found${NC}"
    else
    	mkdir obj
    fi

    if test -d bin; then
    	echo -e "${G}Directrory bin/ found${NC}"
    else
    	mkdir bin
    fi

    if test -L "$out"; then
	    echo -e "${G}Link to executable found${NC}"
    else
	ln -s bin/$out $out
    fi
}

makefile () {

    echo -e "${G}Makefile creation${NC}"

    echo "CC=$cc" > makefile
    echo "LINKER=$linker" >> makefile
    echo "FLAGS=$flags" >> makefile
    echo "OBJ=${obj[*]}" >> makefile
    echo "SRC=${src[*]}" >> makefile

    echo "${out}: \$(OBJ)" >> makefile
    echo -e "\t\$(CC) \$(FLAGS) -o bin/$out \$(OBJ) \$(LINKER)\n" >> makefile

    for((i=0; i<"${#obj[*]}"; i++))
    do
        echo "${obj[$i]}: ${src[$i]}" >> makefile
        echo -e "\t\$(CC) \$(FLAGS) -c ${src[$i]} -o ${obj[$i]} \$(LINKER)\n" >> makefile
    done

    echo -e "clean:" >> makefile
    echo -e "\trm ${obj[*]}\n" >> makefile

    echo "run: ${out}" >> makefile
    echo -e "\t./bin/${out}\n" >> makefile

}

echo -e "${G}Sart Makefile generation${NC}"
variables
requirements
makefile