#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

// Custom string length function
int str_len(const char *str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

// Custom string comparison
int str_cmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

// Custom path builder
void build_path(char *dest, const char *dir, const char *file) {
    while (*dir) *dest++ = *dir++;
    *dest++ = '/';
    while (*file) *dest++ = *file++;
    *dest = '\0';
}

int copy_file(char *source, char *dest) {
    int src_fd = open(source, O_RDONLY);
    if (src_fd < 0) {
        const char *error_msg = "Cannot open source file\n";
        write(STDERR_FILENO, error_msg, str_len(error_msg));
        return -1;
    }

    int dst_fd = creat(dest, 0644);
    if (dst_fd < 0) {
        const char *error_msg = "Cannot create destination file\n";
        write(STDERR_FILENO, error_msg, str_len(error_msg));
        close(src_fd);
        return -1;
    }

    char buffer[4096];
    int bytes_read;
    
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dst_fd, buffer, bytes_read);
    }

    close(src_fd);
    close(dst_fd);
    return 0;
}

int copy_dir(char *source, char *dest) {
    DIR *dir = opendir(source);
    if (!dir) {
        const char *error_msg = "Cannot open source directory\n";
        write(STDERR_FILENO, error_msg, str_len(error_msg));
        return -1;
    }

    mkdir(dest, 0755);
    struct dirent *entry;
    char src_path[1024], dst_path[1024];

    while ((entry = readdir(dir))) {
        if (str_cmp(entry->d_name, ".") == 0 || str_cmp(entry->d_name, "..") == 0)
            continue;

        build_path(src_path, source, entry->d_name);
        build_path(dst_path, dest, entry->d_name);
        copy_file(src_path, dst_path);
    }

    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        const char *usage = "Usage: program [-r] source destination\n";
        write(STDERR_FILENO, usage, str_len(usage));
        return 1;
    }

    if (str_cmp(argv[1], "-r") == 0) {
        if (argc != 4) return 1;
        return copy_dir(argv[2], argv[3]);
    }
    return copy_file(argv[1], argv[2]);
}
