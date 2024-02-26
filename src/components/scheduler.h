
#pragma once

namespace scheduler
{
    enum pipeline_t
    {
        async,
        main,
        server,
        renderer,
        hud,

        count
    };

    struct task_s
    {
        void(*handler)();
        int interval;
        int lastCall;
        bool once;
    };

    class pipeline
    {
    private:
        task_s stack[30];

    public:
        void add(void(*handler)(), int interval, bool once);
        void execute();
    };

    void start();
    void stop();

    void loop(pipeline_t thread, void(*handler)(), int interval);
    void once(pipeline_t thread, void(*handler)(), int interval);
}