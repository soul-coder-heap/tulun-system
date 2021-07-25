#!/bin/bash/
##alias docker=/mnt/c/Program*Files/Docker/Docker/resources/bin/docker.exe
#shopt -s expand_aliases
TRY_NUM=3

function run_docker() {
    local choose_line=$(($1+1))
    containerid=$(docker images | awk 'NR=='$choose_line' {print $1":"$2}')
    echo $containerid
    docker run --rm -it --name $2 -v ${PWD}:/home -w /home $containerid  /bin/bash 

}
function exec_docker(){
    echo "exec_docker"
    docker exec -it $1 /bin/bash
}
function run_docker_way(){ 
    echo "***************************************show_docker*******************************************"
    #docker images
    local images_conut=$(docker images | wc -l)
    images_conut=$[images_conut-1]
    docker images |  awk BEGIN'{N=0}{ if(N == 0){N++;print "num "$0}else{print N++"   "$0}}'
    echo "*********************************************************************************************"
    local num=0
    read -p  "plwase input num choose docker image : " num
    if [[ "$num" =~ ^[0-9]+$ ]] && [ $num -gt 0 ] && [ $num -le $images_conut ];then
        run_docker $num $1
    else
        ((TRY_NUM--))
        if [ $TRY_NUM -ne 0 ];then
            echo "error input,please enter again,$TRY_NUM choice left!"
            run_docker_way $1
        else
            echo "You don't have a chance, stupid!!!"
            exit
        fi
    fi
}
function Main(){
    local name="docker_env_"
    case "$1" in
        "cpp")
            name=$name$1
            echo $name
            ;;
        "python")
            name=$name$1
            echo $name
            ;;
        "ts")
            name=$name$1
            echo $name
            ;;
        "cmake")
            name=$name$1
            echo $name
            ;;
        *)
            echo "paramet : cpp(boost gcc seastar...)"
            echo "          python(python library...)"
            echo "          ts(web env)"
            echo "          cmake(cmake test)"
            exit;;
    esac
    containerid=$(docker ps | grep "$name" | awk '{print $1}')
    if [ -z "$containerid" ];then
        echo "run docker ..."
        run_docker_way $name
    else
        echo "exec docker ..."
        exec_docker $containerid
    fi
    
}
Main $@
#docker run -d -p 8082:80 --name runoob-nginx-test-web -v E:/PHPSpace/docker/nginx/www:/usr/share/nginx/html 
