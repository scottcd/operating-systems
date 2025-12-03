pthread_mutex_t forks[5];

void* philosopher(void* arg) {
    int i = *(int*)arg;
    int left = i;
    int right = (i + 1) % 5;
    while (1) {
        think();
        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);
        eat();
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
    }
}

