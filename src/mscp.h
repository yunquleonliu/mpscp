#ifndef MSCP_H
#define MSCP_H

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <libssh/sftp.h>
#include "pool.h"
#include "bwlimit.h"
#include "path.h"

/* 定义枚举类型 */
enum mscp_severity {
	MSCP_SEVERITY_DEBUG,
	MSCP_SEVERITY_INFO,
	MSCP_SEVERITY_WARN,
	MSCP_SEVERITY_ERROR,
	MSCP_SEVERITY_FATAL,
};

enum mscp_direction {
	MSCP_DIRECTION_L2R,
	MSCP_DIRECTION_R2L,
};

/* 定义统计结构体 */
struct mscp_stats {
	uint64_t total_bytes;
	uint64_t done_bytes;
};

struct mscp_ssh_opts {
	char *login_name;
	char *password;
	char *keyfile;
	int port;
	int ai_family;
	char *config;
	char **options;
	char *identity;
	char *proxyjump;
	char *cipher;
	char *hmac;
	char *compress;
	char *ccalgo;
	int debug_level;
	bool enable_nagle;
	char *passphrase;
};

struct mscp_opts {
	enum mscp_severity severity;
	enum mscp_direction direction;
	int nr_threads;
	char *src;
	char *dst;
	char *netdev;
	uint64_t block_size;
	uint64_t total_size;
	struct mscp_stats stats;
	
	/* 添加缺失的字段 */
	int nr_ahead;
	uint64_t min_chunk_sz;
	uint64_t max_chunk_sz;
	uint64_t buf_sz;
	int max_startups;
	int interval;
	char *coremask;
	uint64_t bitrate;
	bool preserve_ts;
};

struct mscp_thread {
	struct mscp *m;
	sftp_session sftp;

	/* attributes used by copy threads */
	size_t copied_bytes;
	int id;
	int cpu;
	int netdev_index;  /* network device index for this thread */

	/* thread-specific values */
	pthread_t tid;
	int ret;
};

struct mscp {
	struct mscp_opts *opts;
	struct mscp_ssh_opts *ssh_opts;
	pthread_t *threads;
	int nr_threads;
	int running;
	
	/* 添加缺失的字段 */
	char *remote;
	int direction;
	char dst_path[PATH_MAX];
	int *cores;
	int nr_cores;
	sem_t *sem;
	sftp_session first;
	pool *src_pool, *path_pool, *chunk_pool, *thread_pool;
	size_t total_bytes;
	bool chunk_pool_ready;
	struct bwlimit bw;
	struct mscp_thread scan;
};

/* 宏定义 */
#define chunk_pool_is_ready(m) ((m)->chunk_pool_ready)
#define chunk_pool_set_ready(m, b) ((m)->chunk_pool_ready = b)

/* 函数声明 */
struct mscp *mscp_new(void);
void mscp_free(struct mscp *m);
int mscp_start(struct mscp *m);
int mscp_stop(struct mscp *m);
int mscp_wait(struct mscp *m);
struct mscp *mscp_init(struct mscp_opts *o, struct mscp_ssh_opts *s);
int mscp_set_remote(struct mscp *m, const char *remote_host, int direction);
int mscp_connect(struct mscp *m);
int mscp_add_src_path(struct mscp *m, const char *src_path);
int mscp_set_dst_path(struct mscp *m, const char *dst_path);
int mscp_scan(struct mscp *m);
int mscp_scan_join(struct mscp *m);
int mscp_checkpoint_get_remote(const char *pathname, char *remote, size_t len, int *dir);
int mscp_checkpoint_load(struct mscp *m, const char *pathname);
int mscp_checkpoint_save(struct mscp *m, const char *pathname);
int mscp_join(struct mscp *m);
void mscp_cleanup(struct mscp *m);
void mscp_get_stats(struct mscp *m, struct mscp_stats *s);

/* SSH 相关函数声明 */
const char **mscp_ssh_ciphers(void);
const char **mscp_ssh_hmacs(void);

/* 环境变量定义 */
#define ENV_SSH_AUTH_PASSWORD "SSH_AUTH_PASSWORD"
#define ENV_SSH_AUTH_PASSPHRASE "SSH_AUTH_PASSPHRASE"

#endif /* MSCP_H */ 