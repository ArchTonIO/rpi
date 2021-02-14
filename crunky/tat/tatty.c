/* format the filesystem
*/
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <tat.h>


int part_fd; // a file descriptor for the partition

extern u32 part_start; // partition at which the block starts
extern u32 part_size; // number of blocks in partition

int streq(char* cmd, char* str) {
	return strcmp(cmd, str) == 0;
}

int sd_readblock(uint lba, uchar* buffer, uint num)
{
	puts("sd_block called");
	assert(sizeof(off_t)>= sizeof(u32));
	u32 offset = (u32)lba * 512;
	lseek(part_fd, offset, SEEK_SET);
	read(part_fd, buffer, num*512);
	return num*512;

}

int sd_readablock(uint block_num, uchar* buffer)
{
	return sd_readblock(block_num, buffer, 1);
}


int sd_writeablock(uchar* buffer, u32 block_num)
{
	u32 offset = (u32) block_num * 512;
	lseek(part_fd, offset, SEEK_SET);
	return write(part_fd, buffer, 512);
}

void tat_flush_table()
{
	sd_writeablock((uchar*) &tat, part_start); // table goes into first block of partition
}

void tat_store_tat()
{
	lseek(part_fd, 0, SEEK_SET);
	write(part_fd, (void*) &tat, sizeof(tat));
}

void tat_fmt()
{
	assert(part_start);

	u32 reserve = (part_size -1)/NTAES; // the blocks we will reserve for each file


/*
	struct stat statbuf;
	fstat(part_fd, &statbuf);
	u32 size = statbuf.st_size;
	printf("Total size:%d\n", size);

	u32 offset = sizeof(tat);
	u32 file_size = (size -offset)/NTAES;
	printf("Each file is of size %d\n", file_size);
*/
	strncpy(tat.magic, "TATFS01.", sizeof(tat.magic)); // no null-termination
	for(int i=0; i<NTAES; i++) {
		tae_t* tae = &tat.taes[i];
		tae->start = i*reserve + 1;
		tae->res = reserve;
		tae->size = 0;
		tae->flags = 0;
		strcpy(tae->name, "EMPTY");
	}
	tat_flush_table();

}



void tat_store(char* real, char* as)
{
	int slot;
	tae_t* tae;
	for(slot=0; slot<NTAES; slot++) {
		tae = &tat.taes[slot];
		if(tae->flags==0) break;
	}

	if(slot == NTAES) {
		puts("File system full");
		return;
	}

	printf("slot %d, as %s\n", slot, as);
	strcpy(tae->name, as);

	FILE* fp = fopen(real, "r");
	assert(fp);

	char buffer[512];

	//int offset = tae->start;
	//lseek(part_fd, offset, SEEK_SET);
	int nread;
	int size =0;
	u32 offset_block = part_start + tae->start;
	while(nread = fread(buffer, 1, sizeof(buffer), fp)) {
		printf(",");
		sd_writeablock(buffer, offset_block++); 
		//write(part_fd, buffer, nread);
		size += nread;
	}
	//tat_flush();
	fclose(fp);
	tae->size = size;
	tae->flags = 1;
	tat_flush_table();


	//tat_ls();


}

#ifndef max
#define max(a, b) ((a)>(b)? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a)<(b)? (a) : (b))
#endif

void tat_cat(char* path)
{
	int slot;
	tae_t* tae;
	for(slot=0; slot<NTAES; slot++) {
		tae = &tat.taes[slot];
		if((tae->flags & 1) && streq(tae->name, path)) break; 
	}

	if(slot == NTAES) {
		puts("File not found");
		return;
	}

	//lseek(part_fd, tae->start, SEEK_SET);
	char buffer[512];
	int size = tae->size;
	u32 offset_block = part_start + tae->start;
	printf("file size %d\n", size);
	for(;;) {
		if(size == 0) break;
		printf("size = %d\n", size);
		if(size <=0) break;
		sd_readablock(offset_block++, buffer);
		int nread = min(size, 512);
		//ssize_t nread = read(part_fd, buffer, min(size, sizeof(buffer)));
		write(0, buffer, nread);
		if(size<sizeof(buffer)) break;
		size -= sizeof(buffer);
	}

}



int main(int argc, char* argv[])
{

	if(argc<2) {
		puts("insuffient arguments");
		return 1;
	}

	printf("Size of alloaction table is %d\n", sizeof(tat));


#if 0
	char *dev = "tat.fs";
#else
	char *dev = "/dev/loop0";
#endif
	part_fd = open(dev, O_RDWR);
	assert(part_fd != 1);

	assert(sizeof(tat) == 512);

	tat_mount();

	char* cmd = argv[1];
	if(streq(cmd, "fmt")) {
		puts("found format");
		tat_fmt();
	} 
	/*
	else {
		// load tat
		lseek(part_fd, 0, SEEK_SET);
		read(part_fd, &tat, sizeof(tat));
	}
	*/

	if(streq(cmd, "ls")) tat_ls();

	if(streq(cmd, "store")) tat_store(argv[2], argv[3]);

	if(streq(cmd, "cat")) tat_cat(argv[2]);

	if(streq(cmd, "mount")) tat_mount();
	//tat_store_tat();

	close(part_fd);

	return 0;
}
