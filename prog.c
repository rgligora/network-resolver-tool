#include "prog.h"

int main(int argc, char *argv[]){
    int ch;

    bool transportProtocolTCP = false;
    bool transportProtocolUDP = false;
    bool hexOutput = false;
    bool hostByteOrder = false;
    bool networkByteOrder = false;
    bool IPv4 = false;
    bool IPv6 = false;
    bool reverseLookup = false;

    //default
    transportProtocolTCP = true;
    hostByteOrder = true;
    IPv4 = true;

    if(argc == 1){
        errx(1, "prog [-r] [-t|-u] [-x] [-h|-n] [-46] {hostname|IP_address} {servicename|port}");
    }

    while((ch = getopt(argc, argv, "rtuxhn46")) != -1){
        switch (ch) {
            case 't':
                transportProtocolTCP = true;
                transportProtocolUDP = false;
            break;
            case 'u':
                    transportProtocolTCP = false;
                    transportProtocolUDP = true;
                break;
            case 'x':
                hexOutput = true;
                break;
            case 'h':
                hostByteOrder = true;
                networkByteOrder = false;
                break;
            case 'n':
                hostByteOrder = false;
                networkByteOrder = true;
                break;
            case '4':
                IPv4 = true;
                IPv6 = false;
                break;
            case '6':
                IPv4 = false;
                IPv6 = true;
                break;
            case 'r':
                reverseLookup = true;
                break;
            
            default:
                return 1;
        }
    }

    
    if(argc - optind != 2){
        errx(1, "Two arguments are required {hostname|IP_address} {servicename|port}\nprog [-r] [-t|-u] [-x] [-h|-n] [-46] {hostname|IP_address} {servicename|port}");
    }

    

    if(reverseLookup){
        char *IP_address = argv[optind];

        int port = atoi(argv[optind + 1]); 
        char *protocol = transportProtocolTCP ? "tcp" : "udp"; 
        struct servent *service = getservbyport(htons(port), protocol);
        char *serviceName = service->s_name;

        char host[NI_MAXHOST];
        int error;

        if(IPv4){
            struct sockaddr_in sa;
            sa.sin_family = AF_INET;

            if(inet_pton(AF_INET, IP_address, &(sa.sin_addr)) != 1){
                errx(1,"%s is not a valid IP address", IP_address);
            }

            error = getnameinfo((struct sockaddr *)&sa, sizeof(struct sockaddr_in), host, sizeof(host), NULL, 0, NI_NAMEREQD);

            if(error) errx(1, "%s", gai_strerror(error));
            printf("%s (%s) %s\n", IP_address, host, serviceName);
        }else{ //IPv6
            struct sockaddr_in6 sa6;
            sa6.sin6_family = AF_INET6;

            if(inet_pton(AF_INET6, IP_address, &(sa6.sin6_addr)) != 1){
                errx(1,"%s is not a valid IP address", IP_address);
            }

            error = getnameinfo((struct sockaddr *)&sa6, sizeof(struct sockaddr_in6), host, sizeof(host), NULL, 0, NI_NAMEREQD);

            if(error) errx(1, "%s", gai_strerror(error));
            printf("%s (%s) %s\n", IP_address, host, serviceName);
        }

    }else {
        struct addrinfo hints, *res, *p;
        int error;
        char ipstr[INET6_ADDRSTRLEN];
        char portstr[6];
        int port;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = (IPv4) ? AF_INET : AF_INET6;
        hints.ai_flags |= AI_CANONNAME;
        hints.ai_socktype = (transportProtocolTCP) ? SOCK_STREAM : SOCK_DGRAM;

        char *hostname = argv[optind];
        char *serviceName = argv[optind+1];
        error = getaddrinfo(hostname, serviceName,  &hints, &res); 
        if (error) errx(2, "%s", gai_strerror(error));

        p = res;
        while(res){
            if(res->ai_family == AF_INET){
                inet_ntop(res->ai_family, &((struct sockaddr_in *) res->ai_addr)->sin_addr, ipstr, sizeof(ipstr));
                if(networkByteOrder){
                    port = ((struct sockaddr_in *) res->ai_addr)->sin_port;
                }else{
                    port = ntohs(((struct sockaddr_in *) res->ai_addr)->sin_port);
                }
            } else if (res->ai_family == AF_INET6){
                inet_ntop(res->ai_family, &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr, ipstr, sizeof(ipstr));
                if(networkByteOrder){
                    port = ((struct sockaddr_in6 *) res->ai_addr)->sin6_port;
                }else{
                    port = ntohs(((struct sockaddr_in6 *) res->ai_addr)->sin6_port);
                }
            }
            
            snprintf(portstr, sizeof(portstr), "%d", port);
            if(!hexOutput){
                printf("%s (%s) %s\n", ipstr, res->ai_canonname, portstr);
            }else{
                printf("%s (%s) %04X\n", ipstr, res->ai_canonname, port);
            }
            break;
        }
        freeaddrinfo(p);
    }

    return 0;
}
