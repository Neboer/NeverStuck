#include "parse_option.h"

Options *parse_options(int argc, char *argv[])
{
    Options *opt = malloc(sizeof(Options));

    // 设置默认值
    opt->max_stuck_time_sec = -1;
    opt->start_successful_output = NULL;
    opt->arguments = NULL;

    // 定义长选项
    struct option long_options[] = {
        {"max-stuck-time", required_argument, NULL, 't'},
        {"start-successful-output", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0}};

    // 解析选项
    int opt_char;
    while ((opt_char = getopt_long(argc, argv, "t:o:", long_options, NULL)) != -1)
    {
        switch (opt_char)
        {
        case 't': // 最大停滞时间
            opt->max_stuck_time_sec = atoi(optarg);
            break;
        case 'o': // 启动成功输出
            opt->start_successful_output = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s --max-stuck-time <seconds> --start-successful-output <message> -- <command> [arguments...]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // 解析剩余的参数
    if (optind < argc)
    {
        opt->arguments = &argv[optind];
    }
    else
    {
        fprintf(stderr, "Command is missing.\n");
        exit(EXIT_FAILURE);
    }

    if (opt->max_stuck_time_sec <= 0)
    {
        fprintf(stderr, "max stuck time must > 0\n");
        exit(EXIT_FAILURE);
    }
    if (opt->start_successful_output == NULL)
    {
        fprintf(stderr, "no successful output!\n");
        exit(EXIT_FAILURE);
    }

    return opt;
}