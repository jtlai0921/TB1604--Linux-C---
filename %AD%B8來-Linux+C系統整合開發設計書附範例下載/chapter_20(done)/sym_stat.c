#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(void)
{
	struct stat buf; /* 儲存檔案狀態資訊 */
	
	if(stat("sl", &buf) == -1){
		perror("fail to stat");
	exit(1);
}
	
	printf("permission : %d\n", buf.st_mode); /* 列印檔案權限字 */
	printf("inode number : %d\n", buf.st_ino); /* 列印i節點號 */
	printf("device number : %d\n", buf.st_dev); /* 列印檔案系統裝置號 */
	printf("r-device number : %d\n", buf.st_rdev); /* 列印硬件裝置裝置號 */
	printf("link : %d\n", buf.st_nlink); /* 列印硬鏈接數 */
	printf("uid : %d\n", buf.st_uid); /* 列印所有者使用者ID */
	printf("gid : %d\n", buf.st_gid); /* 列印所有者組ID */
	printf("file size : %d\n", buf.st_size); /* 列印檔案的大小 */
	printf("access time : %d\n", buf.st_atime); /* 列印最近一次的存取時間*/
	printf("motify time : %d\n", buf.st_mtime); /* 列印最近一次修改檔案內容的時間 */
	printf("change time : %d\n", buf.st_ctime); /* 列印最近一次修改檔案屬性的時間 */
	printf("buf size : %d\n", buf.st_blksize); /* 列印最適的緩衝區大小 */
	printf("block size : %d\n", buf.st_blocks); /* 列印檔案在外存上佔用的碟塊數 */

	if(stat("test.txt", &buf) == -1){
		perror("fail to stat");
	exit(1);
}

	printf("permission : %d\n", buf.st_mode); /* 列印檔案權限字 */
	printf("inode number : %d\n", buf.st_ino); /* 列印i節點號 */
	printf("device number : %d\n", buf.st_dev); /* 列印檔案系統裝置號 */
	printf("r-device number : %d\n", buf.st_rdev); /* 列印硬件裝置裝置號 */
	printf("link : %d\n", buf.st_nlink); /* 列印硬鏈接數 */
	printf("uid : %d\n", buf.st_uid); /* 列印所有者使用者ID */
	printf("gid : %d\n", buf.st_gid); /* 列印所有者組ID */
	printf("file size : %d\n", buf.st_size); /* 列印檔案的大小 */
	printf("access time : %d\n", buf.st_atime); /* 列印最近一次的存取時間*/
	printf("motify time : %d\n", buf.st_mtime); /* 列印最近一次修改檔案內容的時間 */
	printf("change time : %d\n", buf.st_ctime); /* 列印最近一次修改檔案屬性的時間 */
	printf("buf size : %d\n", buf.st_blksize); /* 列印最適的緩衝區大小 */
	printf("block size : %d\n", buf.st_blocks); /* 列印檔案在外存上佔用的碟塊數 */
	
return 0;
}
