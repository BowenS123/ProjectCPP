# Opdracht - Object Georiënteerde Project (Basis)

## 1. useful and correct class (explain why)
- Waarom: Omdat Factory heeft één verantwoordelijkheid, het beheren van clankers, resources, batterijen, gezondheid, gevechten en logging. De UI communiceert daardoor met één object in plaats van losse helpers of globals.

```cpp
// bestand factory.h
class Factory {
public:
    static constexpr int MAX_HEALTH = 500;

    Factory();
    explicit Factory(std::string name);
    Factory(const Factory& other) = delete;
    ~Factory();

    Factory& operator=(const Factory&) = delete;

    // Production & resources
    void produceClanker(Clanker* clankerPtr);
    bool safeProduceWorker();
    bool produceBattery(int count = 1);

    void updateAll(float dt = 1.0f);
    void update(float dt);

    // Health
    void takeDamage(int dmg);
    void repair(int hp = 10);

    // Resource queries
    void addResources(int delta = 0);
    int getResources() const;
    const std::vector<Clanker*>& getClankers() const;
    int getBatteries() const;
    void addBatteries(int diff);

    // Combat
    std::string defendAgainst(Enemy& enemy);

    const std::string& getName() const;
    unsigned char getId() const;
    int getHealth() const;
    bool isDestroyed() const;
    const std::string& getLogPath() const;
    const Clanker* getFirstActiveClanker() const;

private:
    // Persist a timestamped message so the UI can inspect activity history.
    void log(const std::string& message) const;

    // Identity & overall health
    std::string name;
    unsigned char id;
    int health;

    // Operational state
    std::vector<Clanker*> clankers;
    bool loggingEnabled;
    int resources;
    int batteryStorage;
    unsigned char nextId;
    std::string logPath;
    mutable std::ofstream logFile;
};
```

## 2. useful and correct abstraction (explain why)
- Waarom: Omdat de Factory aan de buitenkant eenvoudige functies aanbiedt zoals produce, defend en repair, terwijl de interne complexiteit (lijsten, resource‑beheer en logging) verborgen blijft. Zo kunnen gebruikers de interne details niet te kennen en kunnen ze de klasse eenvoudig gebruiken.

```cpp
// bestand factory.h
void produceClanker(Clanker* clankerPtr);
bool safeProduceWorker();
std::string defendAgainst(Enemy& enemy);
void repair(int hp = 10);
```

## 3. useful and correct encapsulation (explain why)
- Waarom: Omdat alle interne state privé blijft en enkel aangepast kan worden via gecontroleerde functies met regels. Hierdoor kunnen resources, batterijen en health nooit in ongeldige waarden terechtkomen, wat de objecttoestand veilig en consistent houdt.

```cpp
// bestand factory.cpp
void Factory::addResources(int delta) {
    resources += delta;
    if (resources < 0) {
        resources = 0;
    }
}

void Factory::addBatteries(int diff) {
    batteryStorage += diff;
    if (batteryStorage < 0) {
        batteryStorage = 0;
    }
}

void Factory::repair(int hpValue) {
    health += hpValue;
    if (health > MAX_HEALTH) {
        health = MAX_HEALTH;
    }
    log("Factory repaired by " + std::to_string(hpValue));
}

void Factory::takeDamage(int dmg) {
    if (dmg <= 0) {
        return;
    }

    health -= dmg;
    if (health < 0) {
        health = 0;
    }
    log("Factory damaged for " + std::to_string(dmg));
}
```

## 4. useful and correct inheritance (explain why)
- Waarom: Omdat WorkerClanker, DefenderClanker en ScoutClanker een gedeelde basis hebben in Clanker (zoals naam, HP en energie) en elke specifieke rol alleen zijn eigen work() gedrag hoeft te implementeren. Hierdoor wordt duplicatie vermeden en kunnen nieuwe Clanker-types eenvoudig worden toegevoegd

```cpp
// bestand worker.h
class WorkerClanker final : public Clanker {
public:
    static constexpr int BASE_HP = 100;
    static constexpr int BASE_ENERGY = 100;
    static constexpr int RETALIATION_DAMAGE = 5;

    explicit WorkerClanker(std::string name = "Worker", unsigned char id = 0);
    ~WorkerClanker() override = default;

    void setFactory(Factory& factory);

protected:
    void work() override;

private:
    Factory* factoryRef;
};
```

## 5. useful and correct polymorphism (explain why)
- Waarom: Omdat de fabriek elke Clanker via hetzelfde interface (doWork()) kan aanroepen, terwijl virtual automatisch de juiste work() implementatie van het concrete type uitvoert. Hierdoor hoeft de fabriek niet te controleren welk type Clanker het is.

```cpp
// bestand clanker.cpp
void Clanker::doWork(float dt) {
    (void)dt;
    work();
}

// bestand factory.cpp
void Factory::updateAll(float dt) {
    std::vector<Clanker*> snapshot = clankers;

    for (auto* clanker : snapshot) {
        if (clanker && !clanker->isDestroyed()) {
            clanker->doWork(dt);
        }
    }
}
```

## 6. useful and correct object composition (explain why)
- Waarom: Omdat de MainWindow een Factory bevat en de Factory automatisch leeft zolang het programma actief is. Zo is het duidelijk wie de Factory beheert en blijven er geen losse objecten rondzweven.

```cpp
// bestand mainwindow.h
private:
    Ui::MainWindow *ui;
    ClankerSim::Factory factory{"My Factory"};
    QTimer* gameTimer;
    QTimer* enemySpawnTimer;
    std::vector<ClankerSim::Enemy> enemies;
    int enemySpawnCount = 0;

    void updateUI();
```
