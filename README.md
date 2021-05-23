# soal-shift-sisop-modul-3-A10-2021

## Soal 1

Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini. 

#### Solusi
Menggunakan struct untuk memudahkan penggolongan elemen login dan informasi buku. 

##### A. Saat Client tersambung dengan Server, ada dua pilihan, yaitu Register dan Login. Jika memilih Register, Client input ID dan Password. Jika memilih Login, menyesuaikan ID dan Password yang ada didalam aplikasi Server. Sistem tidak bisa menerima 2 Client yang login dalam waktu bersamaan. Membuat lokasi penyimpanan ID dan Password pada file bernama akun.txt
- Membuat koneksi socket untuk komunikasi antara program Client dan Server. Pesan error akan dikirimkan ke Client apabila mengalami kegagalan pembuatan socket.
- Pada command Register, program server akan membuat file akun.txt untuk tempat akun yang akan dibuat oleh User.
- User melakukan login dengan memasukkan username dan password, lalu program Server akan mengecek apakah sesuai dengan yang ada pada file akun.txt.

##### ```Client```
```c
int sock = 0, valread;
struct sockaddr_in serv_addr;
char *hello = "Hello from client";
char buffer[1024] = {0};
if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
{
	printf("\n Socket creation error \n");
	return -1;
}

serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(PORT);
	
// Convert IPv4 and IPv6 addresses from text to binary form
if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
{
	printf("\nInvalid address/ Address not supported \n");
	return -1;
}

if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
{
	printf("\nConnection Failed \n");
	return -1;
}

	if (!logged_in)
	{
            puts("0. Register");
            puts("1. Login");
            printf("Choose Your Option: ");
            int opt;
            scanf("%d",&opt);
            login.status = opt;
            printf("Username: ");
            char buffer[1010];
            scanf("%s",buffer);
            // printf("\n");
            strcpy(login.id,buffer);
            printf("%s\n",login.id);
            printf("Password: ");
            scanf("%s",buffer);
            strcpy(login.password,buffer);
            printf("%s\n",login.password);
            int snd = send(sock,(void*)&login,sizeof(login),0);
            // printf("Password: ");
            // scanf("%s",login.password);
            char message[110];
            int tmp;
            if (tmp = recv(sock,(void*)&message,sizeof(message),0) < 0)
            {
                perror("Register Failed");
                exit(EXIT_FAILURE);
            }
            printf("%s\n",message);
            if (strcmp(message,"Login Success!") == 0)
            {
                logged_in = 1;
            }
        }
```

##### ```Server```
```c
int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
char *hello = "Hello from server";
	
// Creating socket file descriptor
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
{
	perror("socket failed");
	exit(EXIT_FAILURE);
}

// Forcefully attaching socket to the port 8080
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
{
	perror("setsockopt");
	exit(EXIT_FAILURE);
}
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons( PORT );
	
// Forcefully attaching socket to the port 8080
if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0)
{
	perror("bind failed");
	exit(EXIT_FAILURE);
}
if (listen(server_fd, 3) < 0)
{
        perror("listen");
        exit(EXIT_FAILURE);
}
if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
   (socklen_t*)&addrlen))<0)
{
        perror("accept");
        exit(EXIT_FAILURE);
}

	    creds login;
            int tmp;
            if (tmp = recv(new_socket,(void*)&login,4096,0) < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            if (login.status == REGISTER)
            {
                char message[110];
                FILE* f;
                f = fopen("akun.txt","a+");
                char buffer[1010];
                int ada = 0;
                while (fscanf(f,"%s",buffer) != EOF)
                {
                    // puts("MASUK");
                    printf("%s\n",buffer);
                    const char s[2] = ":";
                    char *token;
                    
                    /* get the first token */
                    token = strtok(buffer, s);
                    char uname[1010];
                    char pw[1010];
                    strcpy(uname,token);
                    
                    /* walk through other tokens */
                    while( token != NULL ) {
                        printf( " %s\n", token );
                        token = strtok(NULL, s);
                        if (strlen(pw) == 0)
                            strcpy(pw,token);
                    }
                    if (strcmp(uname,login.id) == 0)
                    {
                        ada = 1;
                        break;
                    }
                }
		fclose(f);
                if (ada == 1)
                {
                    strcpy(message,"User Already Exist!");
                    if (tmp = send(new_socket, (void*)&message,strlen(message),0) < 0)
                    {
                        perror("Register");
                        exit(EXIT_FAILURE);
                    }
                }
                else
                {
                    f = fopen("akun.txt","a");
                    strcpy(message,"User Registered!");
                    if (tmp = send(new_socket, (void*)&message,strlen(message),0) < 0)
                    {
                        perror("Register");
                        exit(EXIT_FAILURE);
                    }
                    fprintf(f,"%s:%s\n",login.id,login.password);
                    fclose(f);
                }
                continue;
            }
            else
            if (login.status == LOGIN)
            {
                char message[110];
                FILE* f;
                f = fopen("akun.txt","a+");
                char buffer[1010];
                int ada = 0;
                int benar = 0;
                while (fscanf(f,"%s",buffer) != EOF)
                {
                    printf("%s\n",buffer);
                    const char s[2] = ":";
                    char *token;
                    
                    /* get the first token */
                    token = strtok(buffer, s);
                    char uname[1010];
                    char pw[1010];
                    strcpy(uname,token);
                    strcpy(pw,"");
                    
                    /* walk through other tokens */
                    while( token != NULL ) {
                        printf( " %s %d\n", token,(int)strlen(pw));
                        token = strtok(NULL, s);
                        if (strlen(pw) == 0)
                            strcpy(pw,token);
                    }
                    if (strcmp(uname,login.id) == 0)
                    {
                        ada = 1;
                        printf("%s\n",pw);
                        if (strcmp(pw,login.password) == 0)
                            benar = 1;
                        break;
                    }
                }
                fclose(f);
                if (!ada)
                {
                    strcpy(message,"User Not Found!");
                    continue;
                }
                else
                if (!benar)
                {
                    strcpy(message,"Password Incorrect!");
                    continue;
                }
                else
                {
                    strcpy(message,"Login Success!");
                }
                if (tmp = send(new_socket, (void*)&message,strlen(message),0) < 0)
                {
                    perror("Register");
                    exit(EXIT_FAILURE);
                }
                logged_in = 1;
            }
        }
        int command;
        int rcvCommand;
        if (rcvCommand = recv(new_socket,(void*)&command,sizeof(command),0) < 0)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        }   
```

##### B. Sistem memiliki database bernama files.tsv. Isinya adalah path file saat berada di server, publisher dan tahun publikasi. Sistem otomatis membuat folder FILES dan setiap penambahan dan penghapusan file pada folder tersebut memengaruhi isi dari files.tsv.
- Membuat file.tsv pada saat user memasukkan command ```add```.
- Mencatat segala perubahan file pada folder tersebut sesuai dengan format pada soal.

##### C. Client dapat menambah file baru ke dalam server. (Commad Client : add)
- Menggunakan operasi-operasi dasar dari string untuk memproses path file.
- Pada command ini, program server akan membuat files.tsv juga.
- Client memasukkan Publisher, Tahun Publikasi dan Filepath yang nanti akan dicatat dalam files.tsv

##### ```Client```
```c
	if (command == ADD)
	{
                book dt;
                printf("Publisher: ");
                scanf("%s",dt.publisher);
                printf("Tahun Publikasi: ");
                scanf("%d",&dt.tahun);
                printf("Filepath: ");
                scanf("%s",dt.filePath);
                int sendAdd;
                if (sendAdd = send(sock,(void*)&dt,sizeof(dt),0) < 0)
                {
                    perror("Send add");
                    exit(EXIT_FAILURE);
                }
                char message[110];
                int rcvMsg;
                if (rcvMsg = recv(sock,(void*)&message,sizeof(message),0) < 0)
                {
                    perror("Add Failed");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",message);
            }
```

##### ```Server```
```c
 	if (command == ADD)
        {
            book dt;
            int rcvDt;
            if (rcvDt = recv(new_socket,(void*)&dt,4096,0) < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            char message[510];
            strcpy(message,"Successfully Added Book");
            int sndMsg;
            if (sndMsg = send(new_socket, (void*)&message,strlen(message),0) < 0)
            {
                perror("Add book");
                exit(EXIT_FAILURE);
            }
            char buff[510];
            strcpy(buff,dt.filePath);
            char* fileName = strrchr(buff,'/');
            FILE* f;
            f = fopen("files.tsv","a");
            fprintf(f,"%s\t%d\t%s\n",dt.publisher,dt.tahun,dt.filePath);
            fclose(f);
            printf("%s\n",fileName);
            memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
            char dir[510];
            strcpy(dir,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021/files/");
            strcat(dir,fileName);
            printf("%s -> %s\n",dt.filePath,dir);
            FILE* from;
            from = fopen(dt.filePath,"r");
            FILE* to;
            to = fopen(dir,"w");
            char trfBuf[BUFSIZ];
            size_t size;
            while (size = fread(trfBuf, 1, BUFSIZ, from)) 
            {
                fwrite(trfBuf, 1, size, to);
            }
            fclose(from);
            fclose(to);
        }
```

##### D. Client dapat mendownload file di Server. Server harus melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. (Command Client : download)
- Menggunakan operasi-operasi dasar dari string untuk memproses path file.
- Client memasukkan nama file yang akan di download.
- Pada command ini, Program akan mengecek apakah file yang akan di download ada atau tidak. 
- Program akan memperbarui path dari file setelah di download.

##### ```Client```
```c
	    else if (command == DOWNLOAD)
            {
                printf("Input file name:");
                char downloadFileName[510];
                scanf("%s",downloadFileName);
                int sendDwnld;
                if (sendDwnld = send(sock,(void*)&downloadFileName,sizeof(downloadFileName),0) < 0)
                {
                    perror("Send Download");
                    exit(EXIT_FAILURE);
                }
                char message[110];
                int rcvMsg;
                if (rcvMsg = recv(sock,(void*)&message,sizeof(message),0) < 0)
                {
                    perror("Download Failed");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",message);
            }
```

##### ```Server```
```c
	if (command == DOWNLOAD)
        {
            char dwnldFileName[510];
            int rcvDwnld;
            if (rcvDwnld = recv(new_socket,(void*)&dwnldFileName,4096,0) < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            FILE* tsv;
            tsv = fopen("files.tsv","r");
            book rd;
            int found = 0;
            while (fscanf(tsv,"%s %d %s",rd.publisher,&rd.tahun,rd.filePath) != EOF)
            {
                char bfr[510];
                strcpy(bfr,rd.filePath);
                char* fileName = strrchr(bfr,'/');
                memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                if (strcmp(fileName,dwnldFileName) == 0)
                {
                    found = 1;
                    break;
                }
            }
            fclose(tsv);
            char dwnldMsg[510];
            if (!found)
            {
                strcpy(dwnldMsg,"Book Not Found!");
                continue;
            }
            else
                strcpy(dwnldMsg,"Book Downloaded!");
            int dwnldSnd;
            if (dwnldSnd = send(new_socket, (void*)&dwnldMsg,strlen(dwnldMsg),0) < 0)
            {
                perror("Download Book");
                exit(EXIT_FAILURE);
            }
            char srcDir[510];
            strcpy(srcDir,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021/files/");
            strcat(srcDir,dwnldFileName);
            char destDir[510];
            strcpy(destDir,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021/");
            strcat(destDir,dwnldFileName);
            FILE* from;
            from = fopen(srcDir,"r");
            FILE* to;
            to = fopen(destDir,"w");
            printf("%s -> %s\n",srcDir,destDir);
            char trfBuf[BUFSIZ];
            size_t size;
            while (size = fread(trfBuf, 1, BUFSIZ, from)) 
            {
                fwrite(trfBuf, 1, size, to);
            }
            fclose(from);
            fclose(to);
        }
```

##### E. Client dapat menghapus file yang tersimpan di Server, dengan cara me-rename menjadi 'old-NameFile.ekstensi'. (Command Client : delete)
- Menggunakan operasi-operasi dasar dari string untuk memproses path file.
- Client memasukkan nama file yang akan di delete, bukan benar-benar dihapus, tetapi me-rename file nya.
- Pada command ini, Program akan mengecek apakah file yang akan di delete ada atau tidak. 

##### ```Client```
```c
	    else if (command == DELETE)
            {
                printf("Input File Name:");
                char deleteFileName[510];
                scanf("%s",deleteFileName);
                int sendDel;
                if (sendDel = send(sock,(void*)&deleteFileName,sizeof(deleteFileName),0) < 0)
                {
                    perror("Send Delete");
                    exit(EXIT_FAILURE);
                }
                char message[110];
                int rcvMsg;
                if (rcvMsg = recv(sock,(void*)&message,sizeof(message),0) < 0)
                {
                    perror("Download Failed");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",message);
            }
```

##### ```Server```
```c
	if (command == DELETE)
        {
            char delFileName[510];
            int rcvDel;
            if (rcvDel = recv(new_socket,(void*)&delFileName,4096,0) < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            book simpan[510];
            book rd;
            int ix;
            FILE* tsv;
            tsv = fopen("files.tsv","r");
            int found = 0;
            while (fscanf(tsv,"%s %d %s",rd.publisher,&rd.tahun,rd.filePath) != EOF)
            {
                char bfr[510];
                strcpy(bfr,rd.filePath);
                char* fileName = strrchr(bfr,'/');
                memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                if (strcmp(fileName,delFileName) == 0)
                {
                    found = 1;
                    break;
                }
                simpan[ix++] = rd;
            }
            fclose(tsv);
            char delMsg[510];
            if (!found)
            {
                strcpy(delMsg,"Book Not Found!");
                continue;
            }
            else
                strcpy(delMsg,"Book Deleted!");
            int delSnd;
            if (delSnd = send(new_socket, (void*)&delMsg,strlen(delMsg),0) < 0)
            {
                perror("Download Book");
                exit(EXIT_FAILURE);
            }
            char srcDir[510];
            strcpy(srcDir,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021/files/");
            strcat(srcDir,delFileName);
            char destDir[510];
            strcpy(destDir,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021/files/old-");
            strcat(destDir,delFileName);
            rename(srcDir,destDir);
            FILE* overwrite;
            overwrite = fopen("files.tsv","w");
            for (int i = 0 ; i < ix ; i++)
            {
                char bfr[510];
                strcpy(bfr,simpan[i].filePath);
                char* fileName = strrchr(bfr,'/');
                memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                if (strcmp(fileName,delFileName) == 0) continue;
                // puts("masuk");
                fprintf(overwrite,"%s\t%d\t%s\n",simpan[i].publisher,simpan[i].tahun,simpan[i].filePath);
            }
            fclose(overwrite);
        }
```

##### F. Client dapat melihat semua isi files.tsv. (Command Client : see)
- Menggunakan operasi-operasi dasar dari string untuk memproses path file.
- Pada command ini, program server mengirimkan isi dari file files.tsv dan pada program Client menampilkan sesuai dengan format pada soal.

##### ```Client```
```c
	    else if (command == SEE)
            {
                book simpan[510];
                int seeMsg;
                if (seeMsg = recv(sock,(void*)&simpan,sizeof(simpan),0) < 0)
                {
                    perror("See Failed");
                    exit(EXIT_FAILURE);
                }
                for (int i = 0 ; i < 510 ; i++)
                {
                    if (simpan[i].tahun == 0) break;
                    book rd = simpan[i];
                    char bfr[510];
                    strcpy(bfr,rd.filePath);
                    char* fileName = strrchr(bfr,'/');
                    memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                    printf("Nama: %s\n",fileName);
                    printf("Publisher: %s\n",simpan[i].publisher);
                    printf("Tahun Publishing: %d\n",simpan[i].tahun);
                    char buff2[510];
                    strcpy(buff2,fileName);
                    char* extension = strrchr(buff2,'.');
                    memmove(&extension[0],&extension[1],strlen(extension) - 0);
                    printf("Ekstensi File: %s\n",extension);
                    printf("Filepath: %s\n",simpan[i].filePath);
                    printf("\n");
                }
            }
```

##### ```Server```
```c
	if (command == SEE)
        {
            book simpan[510];
            book rd;
            int ix;
            FILE* tsv;
            tsv = fopen("files.tsv","r");
            int found = 0;
            while (fscanf(tsv,"%s %d %s",rd.publisher,&rd.tahun,rd.filePath) != EOF)
            {
                char bfr[510];
                strcpy(bfr,rd.filePath);
                char* fileName = strrchr(bfr,'/');
                memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                simpan[ix++] = rd;
            }
            fclose(tsv);
            int seeSnd;
            if (seeSnd = send(new_socket, (void*)&simpan,sizeof(simpan),0) < 0)
            {
                perror("See");
                exit(EXIT_FAILURE);
            }
        }
```

##### G. Client juga dapat melakukan pencarian dengan memberikan suatu string. Hasilnya adalah semua nama file yang mengandung string tersebut. (Command Client : find)
- Menggunakan operasi-operasi dasar dari string untuk memproses path file.
- Client memasukkan nama file yang ingin dicari.
 
##### ```Client```
```c
	    else if (command == FIND)
            {
                printf("Input Pattern:");
                char pattern[510];
                scanf("%s",pattern);
                int sendFnd;
                if (sendFnd = send(sock,(void*)&pattern,sizeof(pattern),0) < 0)
                {
                    perror("Send Find");
                    exit(EXIT_FAILURE);
                }
                book simpan[510];
                int fndMsg;
                if (fndMsg = recv(sock,(void*)&simpan,sizeof(simpan),0) < 0)
                {
                    perror("Find Failed");
                    exit(EXIT_FAILURE);
                }
                for (int i = 0 ; i < 510 ; i++)
                {
                    if (simpan[i].tahun == 0) break;
                    book rd = simpan[i];
                    char bfr[510];
                    strcpy(bfr,rd.filePath);
                    char* fileName = strrchr(bfr,'/');
                    memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                    printf("%s\n",fileName);
                }
            }
```

##### ```Server```
```c
	if (command == FIND)
        {
            char pattern[510];
            int rcvFind;
            if (rcvFind = recv(new_socket,(void*)&pattern,4096,0) < 0)
            {
                perror("recv Find");
                exit(EXIT_FAILURE);
            }
            book simpan[510];
            book rd;
            int ix;
            FILE* tsv;
            tsv = fopen("files.tsv","r");
            int found = 0;
            while (fscanf(tsv,"%s %d %s",rd.publisher,&rd.tahun,rd.filePath) != EOF)
            {
                char bfr[510];
                strcpy(bfr,rd.filePath);
                char* fileName = strrchr(bfr,'/');
                memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                if (strstr(fileName,pattern) != NULL)
                {
                    simpan[ix++] = rd;
                }
            }
            fclose(tsv);
            int fndSnd;
            if (fndSnd = send(new_socket, (void*)&simpan,sizeof(simpan),0) < 0)
            {
                perror("Find");
                exit(EXIT_FAILURE);
            }
        }
```

##### H. Membuat suatu log untuk server yang bernama running.log
#### Kendala
- Membuat socket Client dengan Server
- Terkadang Client dan Server tidak sinkron

## Soal 2

Crypto (kamu) adalah teman Loba. Suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya.

#### Solusi
##### A. Membuat program perkalian Matriks (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka)
- User melakukan penginputan manual ke Matriks yang ukurannya telah ditentukan (Matriks 1 dan 2).
- Melakukan perkalian antar dua matriks tersebut.
- Pada program ini, terdapat 3 thread yang digunakan, yaitu untuk menampilkan Matriks 1 dan 2 serta fungsi perkalian matriks tersebut.

```c
void *showmatriks1(void *arg)
{
    int i,j,k;
    printf("Matriks 1 %dx%d:\n", R, RC);
	for(i=0; i < R; i++){
		for(j=0; j < RC; j++)
			printf("%d ",matriks1[i][j] );
		printf("\n");
	}
	return NULL;
}

void *showmatriks2(void *arg)
{
    int i,j,k;
    printf("\nMatriks 2 %dx%d:\n", RC, C);
	for(i=0; i < RC; i++){
		for(j=0; j < C; j++)
			printf("%d ",matriks2[i][j] );
		printf("\n");
	}
	return NULL;
}

void *perkalian(void *arg)
{
	for(int i=0;i<R;i++)
	{
		for(int j=0;j<C;j++)
		{
			for(int k=0;k<RC;k++)
			{
				res += matriks1[i][k] * matriks2[k][j];
			}
            nilai[i][j] = res; 
            res =0;
		}
	}
	return NULL;
}
```

- Thread tersebut berjalan pada setiap perulangan sesuai dengan ukuran Matriksnya.
- Shared memory digunakan karena Matriks hasil perkalian tersebut akan digunakan pada soal berikutnya.

```c
pthread_create(&thread1, NULL, showmatriks1, NULL);
pthread_join(thread1,NULL);

pthread_create(&thread2, NULL, showmatriks2, NULL);
pthread_join(thread2,NULL);

printf("\n");

for(int i=0;i<R;i++){
	for(int j=0;j<C;j++){
		nilai[i][j] = 0;
	}
	pthread_create(&thread3, NULL, perkalian, NULL);
	pthread_join(thread3,NULL);
}
```
Output:

![soal2a](https://user-images.githubusercontent.com/73766214/119256494-340e6780-bbeb-11eb-9ce7-cfbca0734a7d.png)

##### B. Membuat program dengan menggunakan Matriks Output dari program sebelumnya (Shared Memory). Kemudian Matriks tersebut dilakukan perhitungan dengan Matriks baru dari inputan user. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya matriks dengan menggunakan Thread untuk perhitungan setiap cell nya (dari paling besar ke paling kecil). 
- Menggunakan Shared Memory untuk memakai Matriks hasil perkalian dari soal sebelumnya.
- User melakukan penginputan Matriks yang nantinya akan diproses dengan Matriks pada soal sebelumnya.
- Menggunakan Thread sebanyak ukuran Matriksnya (4x6) untuk perhitungan di setiap cell nya, dengan variabel ```zero``` untuk penanda apakah elemen di suatu matriks adalah 0. Menghitung selisih dari Matriks a (Matriks hasil perkalian soal sebelumnya) dan Matriks b (input user), lalu mengeksekusi Thread yang memanggil fungsi ```finalmatriks``` sesuai dengan kondisi pada soal :

```c
pthread_t thread_id[R*C];
int cnt=0;
for(int i = 0; i < R; i++){
    for(int j = 0; j < C; j++){
      	zero=0;
        ll *val = malloc(sizeof(ll[4][6]));

        *val = value[i][j];
        beda = value[i][j] - nilaibaru[i][j];

        if(value[i][j]==0 || nilaibaru[i][j]==0) zero=1;

        pthread_create(&thread_id[cnt], NULL, &finalmatriks, val);
        sleep(1);
        cnt++;
    }
    printf("\n");
}
for (int i = 0; i<cnt; i++){
	pthread_join(thread_id[i], NULL);
}
```

```c
void *finalmatriks(void* argv){
	ll n = *(ll*)argv;
	if(zero){
		printf("0 ");
	}
	else if(beda<1){
		printf("%lld ", factorial(n));
	} 
	else printf("%lld ", factorial2(n));
}
```

- Fungsi ```factorial``` untuk menghitung hasil faktorial yang memenuhi kondisi kedua (b > a -> a!).

```c
ll factorial(int n) {
    if (n == 0) return 1;
    return n*factorial(n-1);
}
```

- Fungsi ```factorial2``` untuk menghitung hasil faktorial yang memenuhi kondisi pertama (a >= b -> a!/(a-b)!)

```c
ll factorial2(int n){
	if (n == beda) return 1;
        return n*factorial2(n-1);
}
```

Output:

![soal2b](https://user-images.githubusercontent.com/73766214/119256526-4b4d5500-bbeb-11eb-8a68-bd3dd7ab9f61.png)

##### C. Membuat program untuk mengecek 5 proses teratas yang memakan resource komputer.
- Menggunakan IPC Pipes untuk mengeksekusi perintah ```ps aux | sort -nrk 3,3 | head -5```.
- Menggunakan 2 Pipes. Dilakukan fork pada ```pid_t p1```. Write end pada Pipe1 akan dijalankan untuk mengeksekusi perintah ```ps aux``` pada child p1.

```c
//child p1
else{ //p1 == 0
	dup2(pipe1[1], 1);

	close(pipe1[0]);
        close(pipe1[1]);

        char *arg2[] = {"ps", "aux",NULL};
        execvp("/bin/ps", arg2);
} 
```

- Pada parent p1, dilakukan fork pada ```pid_t p2```, Read end pada Pipe1 dan Write end pada Pipe2 dijalankan untuk mengeksekusi perintah ```sort -nrk 3,3``` pada child p2.

```c
	else{ //child p2
            //Read isi dari Pipe1
            dup2(pipe1[0], 0); 
            //Write ke Pipe 2
            dup2(pipe2[1], 1);

            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            char *arg2[] = {"sort", "-nrk", "3.3",NULL};
            execvp("/bin/sort", arg2);
    	}
```

- Read end pada Pipe2 dijalankan untuk mengeksekusi perintah ```head -5``` pada parent p2.

```c
	else if(p2 > 0){//parent p2
            close(pipe1[0]);
            close(pipe1[1]);

            dup2(pipe2[0], 0);
        
            close(pipe2[0]);
            close(pipe2[1]);

            char *arg1[] = {"head", "-5", NULL};
            execvp("/usr/bin/head", arg1);
            exit(0);
        }
```

Output:

![soal2c](https://user-images.githubusercontent.com/73766214/119256553-67e98d00-bbeb-11eb-9b11-448ac391f254.png)

#### Kendala
- Agak kesulitan untuk menghitung setiap cell menggunakan thread
- Sedikit bingung mengenai penggunaan IPC Pipes

## Soal 3

Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

#### Solusi
Menggunakan struct untuk memudahkan pemrosesan path dari file. Menggunakan fungsi ```listFilesRecursively(path)``` untuk nomor 3b dan 3c.

```c
void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if (dp->d_type == DT_REG)
            {
                struct arguments params;
                strcpy(params.filePath,path);
                strcpy(params.curPath,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021");
                if (strcmp(path,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021/soal3.c") != 0)
                {
                    pthread_create(&id[ix],NULL,categoryThread,(void*)&params);
                    sleep(1);
                    ix++;
                }
            }
            else
            if (dp->d_name[0] != '.')
                listFilesRecursively(path);
        }
    }

    closedir(dir);
}
```

##### A. Program menerima opsi ```-f``` untuk mengkategorikan sebanyak yang diinginkan oleh pengguna.
- Menggunakan operasi-operasi dasar dari string untuk memproses pengkategorian file sesuai ekstensi file nya ke suatu directory.
- Membuat Thread untuk setiap file yang akan dikategorikan dan di-join-kan agar bisa berjalan bersamaan.

```c
if (strcmp(argv[1],"-f") == 0)
    {
        for (int i = 2 ; i < argc ; i++)
        {
            struct arguments params;
            strcpy(params.filePath,argv[i]);
            strcpy(params.curPath,cwd);
            // printf("%d -> %s\n",i,argv[i]);
            // printf("%s %s\n",params.filePath,params.curPath);
            // puts("==========================\n");
            if (is_regular_file(argv[i]))
            {
                pthread_create(&id[ix],NULL,categoryThread,(void*)&params);
                printf("File %d: Berhasil Dikategorikan\n", i - 1);
                sleep(1);
                ++ix;
            }
            else
                printf("File %d: Sad, gagal :(\n",i - 1);
        }
        for (int i = 0 ; i < ix ; i++)
            pthread_join(id[i],NULL);
    }
```
- Fungsi ```categoryThread``` untuk menampilkan path.

```c
void *categoryThread(void *ptr)
{
    puts("masuk");
    struct arguments params = *(struct arguments*)ptr;
    printf("%s %s ini di fungsi thread\n",params.filePath,params.curPath);
    categorize(params.filePath,params.curPath);
    pthread_exit(0);
}
```

- Fungsi ```categorize``` untuk mengkategorikan file sesuai ekstensinya.

```c
void categorize(char* filePath,char* curPath)
{
    // printf("%s %s masuk categorize\n",filePath,curPath);
    char buff[510];
    strcpy(buff,filePath);
    char* fileName = strrchr(filePath,'/');
    printf("%s\n",fileName);
    memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
    if (fileName[0] == '.')
    {
        // puts("masuk sini");
        mkdir("hidden",0777);
        strcat(curPath,"/");
        strcat(curPath,"hidden/");
        strcat(curPath,fileName);
        printf("%s %s\n",curPath,buff);
        rename(buff,curPath);
        return;
    }
    char buff2[510];
    strcpy(buff2,fileName);
    char* extension = strrchr(buff2,'.');
    memmove(&extension[0],&extension[1],strlen(extension) - 0);
    if (strlen(extension) > 0)
    {
        mkdir(extension,0777);
        strcat(curPath,"/");
        strcat(curPath,extension);
        strcat(curPath,"/");
        strcat(curPath,fileName);
        printf("%s %s\n",curPath,buff);
        rename(buff,curPath);
    }
    else
    {
        mkdir("unknown",0777);
        strcat(curPath,"/");
        strcat(curPath,"unknown/");
        strcat(curPath,fileName);
        printf("%s %s\n",curPath,buff);
        rename(buff,curPath);
    }
}
```
Output:

![soal3a](https://user-images.githubusercontent.com/73766214/119256595-a3845700-bbeb-11eb-960e-7490c9e391bc.png)

##### B. Program dapat menerima opsi ```-d``` untuk melakukan pengkategorian pada suatu directory. User hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin.
- Sebenarnya tidak jauh beda dengan soal nomor 3a, hanya saja user dapat mengkategorikan file pada directory yang user inginkan.

```c
else if (strcmp(argv[1],"-d") == 0)
    {
        puts("masuk");
        listFilesRecursively(argv[2]);
    }
```
Output:

![soal3b](https://user-images.githubusercontent.com/73766214/119256609-b4cd6380-bbeb-11eb-92b3-916c27692402.png)

##### C. Program menerima opsi ```*```. Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.
- Sebenarnya tidak jauh beda dengan soal nomor 3a, hanya saja pengkategorian file dilakukan pada working directory, yaitu folder soal3 itu sendiri.

```c
else if (strcmp(argv[1],"-*") == 0)
    {
        puts("masuk");
        listFilesRecursively("/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021");
    }
```

##### D. Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.
##### E. Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

#### Kendala
- Sedikit bingung penggunaan rekursif pada program
