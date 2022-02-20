function expose_module_to_global(module: object) {
    Object.keys(module).forEach((key: string) =>
    {
        eval(key + "=" + module[key]);
    });
}
