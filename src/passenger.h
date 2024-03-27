class Passenger
{
public:
    Passenger(int id, int weight);
    int get_id() const;
    int get_weight() const;
private:
    int id;
    int weight;
};