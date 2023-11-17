#include "storage_server.h"

int main()
{
    // tcp client
    // int sock = 0;
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        /* code */
        perror("Error in socket() function call: ");
        exit(1);
    }
    int server_port_number = 5572;
    char *ip_address = "127.0.0.1";
    char buffer[4096];
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port_number);
    server_address.sin_addr.s_addr = inet_addr(ip_address);

    int connect_success = connect(sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connect_success == -1)
    {
        /* code */
        perror("Error in connect() function call: ");
        exit(1);
    }

    struct data_of_ss initial_data_of_ss;
    scanf("%d", &initial_data_of_ss.number_of_paths);
    initial_data_of_ss.port_number = 6677; // hard coded for time being . Will change this later
    initial_data_of_ss.paths[initial_data_of_ss.number_of_paths];
    for (int i = 0; i < initial_data_of_ss.number_of_paths; i++)
    {
        initial_data_of_ss.paths[i].permissions = 1; // hard coded for time being . Will change this later
        printf("Enter path %d: ", i + 1);
        scanf("%s", initial_data_of_ss.paths[i].path);
        initial_data_of_ss.paths[i].path[strlen(initial_data_of_ss.paths[i].path)] = '\0';
        printf("%s\n", initial_data_of_ss.paths[i].path);
    }
    for (int i = initial_data_of_ss.number_of_paths; i < MAX_PATHS; i++)
    {
        initial_data_of_ss.paths[i].permissions = 0;
        initial_data_of_ss.paths[i].path[0] = '\0';
    }

    int err_check = send(sock, &initial_data_of_ss, sizeof(initial_data_of_ss), 0);
    printf("Data sent to naming server\n");

    int socket_nm = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_nm == -1)
    {
        perror("Error in socket() function call: ");
        exit(1);
    }

    struct sockaddr_in server_address_nm;

    memset(&server_address_nm, 0, sizeof(server_address_nm));
    server_address_nm.sin_family = AF_INET;
    server_address_nm.sin_port = htons(6677);
    server_address_nm.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(socket_nm, (struct sockaddr *)&server_address_nm, sizeof(server_address_nm)) == -1)
    {
        perror("Error in bind() function call: ");
        exit(1);
    }

    if (listen(socket_nm, 10) == -1)
    {
        perror("Error in listen() function call: ");
        exit(1);
    }

    while (1)
    {
        int client_socket_nm;
        struct sockaddr_in client_address_nm;
        int client_address_length_nm = sizeof(client_address_nm);
        client_socket_nm = accept(socket_nm, (struct sockaddr *)&client_address_nm, &client_address_length_nm);
        if (client_socket_nm == -1)
        {
            perror("Error in accept() function call: ");
            exit(1);
        }

        char *buffer1 = (char *)malloc(4096);
        int bytes_received = recv(client_socket_nm, buffer1, 4096, 0);
        if (bytes_received == -1)
        {
            perror("Error in recv() function call: ");
            exit(1);
        }
        char *token = strtok(buffer1, " ");

        if (strcmp(token, "makedir") == 0)
        {
            char *dirname = strtok(NULL, " ");
            char *path = strtok(NULL, " ");
            makedirss(dirname, path, client_socket_nm);
        }
        else if (strcmp(token, "deletedir") == 0)
        {
            char *path = strtok(NULL, " ");
            deletedirss(path, client_socket_nm);
        }
        else if (strcmp(token, "makefile") == 0)
        {
            char *filename = strtok(NULL, " ");
            char *path = strtok(NULL, " ");
            makefiless(filename, path, client_socket_nm);
        }
        else if(strcmp(token,"deletefile")==0)
        {
            char *path = strtok(NULL, " ");
            deletefiless(path, client_socket_nm);
        }

        close(client_socket_nm);
    }

    // close(sock);

    return 0;
}