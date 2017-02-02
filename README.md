## Instructions

* Create Docker container initially


    docker run -d -p 3022:22 -p 7777:7777 -p 9999:9999 -h server.cornell.edu   --security-opt seccomp:unconfined  --name server ebagdasa/cs5450_p1:latest 

* Start stopped Docker container:


    docker start [CONTAINER_NAME | CONTAINER_ID]

* Log into the Docker container:


    ssh -p 3022 root@127.0.0.1

## References
* Tutorial of development environment: https://pages.github.coecis.cornell.edu/cs5450/website/assignments/p1/docker.html