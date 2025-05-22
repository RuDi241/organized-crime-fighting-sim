#ifndef INFO_UNIT_H
#define INFO_UNIT_H


class InfoUnit {
    public:
        InfoUnit(int ID, int importance);
        InfoUnit() = delete;

        int getID() const;
        int getImportance() const;
        

    private:
        int ID;
        int importance;
};

#endif