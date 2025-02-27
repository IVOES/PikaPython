#include "test_common.h"
TEST_START

#include "PikaCompiler.h"
// TEST(packtool, unpack) {

    //PIKA_RES res = pikafs_unpack_files("test/out/packout/a0424.pack", "test/out/unpackout/");

//     EXPECT_EQ(res, PIKA_RES_OK);
// }

TEST(packtool, packfiles) {

    PikaMaker* maker = New_PikaMaker();
    PIKA_RES ret = PIKA_RES_OK;

    pikaMaker_linkRaw_New(maker, "test/out/file3.txt", "/txt-file");
    pikaMaker_linkRaw_New(maker, "test/out/G.bmp", "/bmp-file");

    // create "./test/out/packout" path if not exist
    mkdir("./test/out/packout", 0777);
    ret = pikaMaker_linkCompiledModulesFullPath(maker, "./test/out/packout/a0424.pack");

    pikaMaker_deinit(maker);
    EXPECT_EQ(ret, PIKA_RES_OK);
}

TEST(packtool, packread) {
    size_t n = 0;
    //Arg* fileArg = NULL;
    pikafs_FILE* pack_file = pikafs_fopen_pack("test/out/packout/a0424.pack", "file3.txt");
    if (NULL == pack_file) {
        pika_platform_printf("open file: %s error\r\n", "test/out/packout/a0424.pack");
    }

    mkdir("./test/out/unpackout", 0777);
    FILE* file = pika_platform_fopen("test/out/unpackout/file3_test.txt", "wb+");
    if (NULL == file) {
        pika_platform_printf("open file: %s error\r\n", "test/out/unpackout/file3_test.txt");
    }

    n = pika_platform_fwrite(pack_file->addr, pack_file->size , 1, file);

    arg_deinit(pack_file->farg);
    pikaFree(pack_file, sizeof(pikafs_FILE));
    pika_platform_fclose(file);
    pack_file = NULL;
    EXPECT_NE(n, 0);
}

TEST(packtool, packreadErr) {
    pikafs_FILE* pack_file = pikafs_fopen_pack("test/out/packout/0425.pack", "file3.txt");
    if (NULL == pack_file) {
        pika_platform_printf("open file: %s error\r\n", "test/out/packout/a0424.pack");
    }
    
    EXPECT_TRUE(!pack_file);
}

TEST_END