/* SPDX-License-Identifier: GPL-3.0-only */
#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <mscp.h>

/* message print. printed messages are passed to application via msg_fd */
void set_print_severity(int severity);
int get_print_severity();

#define __print_debug(fmt, ...) fprintf(stdout, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define __print_info(fmt, ...) fprintf(stdout, "[INFO] " fmt "\n", ##__VA_ARGS__)
#define __print_notice(fmt, ...) fprintf(stdout, "[NOTICE] " fmt "\n", ##__VA_ARGS__)
#define __print_warn(fmt, ...) fprintf(stderr, "[WARN] " fmt "\n", ##__VA_ARGS__)
#define __print_err(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

#define pr_debug(fmt, ...) __print_debug(fmt, ##__VA_ARGS__)
#define pr_info(fmt, ...) __print_info(fmt, ##__VA_ARGS__)
#define pr_notice(fmt, ...) __print_notice(fmt, ##__VA_ARGS__)
#define pr_warn(fmt, ...) __print_warn(fmt, ##__VA_ARGS__)
#define pr_err(fmt, ...) __print_err(fmt, ##__VA_ARGS__)

/* 添加函数名和行号的调试宏 */
#define __print_debug_func(func, line, fmt, ...) fprintf(stdout, "[DEBUG] [%s:%d] " fmt "\n", func, line, ##__VA_ARGS__)
#define __print_info_func(func, line, fmt, ...) fprintf(stdout, "[INFO] [%s:%d] " fmt "\n", func, line, ##__VA_ARGS__)
#define __print_err_func(func, line, fmt, ...) fprintf(stderr, "[ERROR] [%s:%d] " fmt "\n", func, line, ##__VA_ARGS__)

#define pr_debug_func(fmt, ...) __print_debug_func(__func__, __LINE__, fmt, ##__VA_ARGS__)
#define pr_info_func(fmt, ...) __print_info_func(__func__, __LINE__, fmt, ##__VA_ARGS__)
#define pr_err_func(fmt, ...) __print_err_func(__func__, __LINE__, fmt, ##__VA_ARGS__)

#endif /* _PRINT_H_ */
