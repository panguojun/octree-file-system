/**                     【几何体表格】      
*/
#define VLIST(clsname)    vector<shared_ptr<clsname>>
namespace GDB2d {

    class Geometry2D {
    public:
        virtual string type() const = 0;
        virtual double area() const = 0;
        virtual double volume() const = 0;
        virtual string serialize() const = 0;
        static shared_ptr<Geometry2D> deserialize(const string& s);
    };

    class Circle : public Geometry2D {
    private:
        double radius;
    public:
        Circle(double r) : radius(r) {}
        string type() const override { return "circle"; }
        double area() const override { return 3.14 * radius * radius; }
        double volume() const override { return 0; }
        string serialize() const override {
            ostringstream oss;
            oss << "circle " << radius;
            return oss.str();
        }
        static shared_ptr<Circle> deserialize(const string& s) {
            istringstream iss(s);
            string type;
            double r;
            iss >> type >> r;
            return make_shared<Circle>(r);
        }
    };

    class Rectangle : public Geometry2D {
    private:
        double width;
        double height;
    public:
        Rectangle(double w, double h) : width(w), height(h) {}
        string type() const override { return "rect"; }
        double area() const override { return width * height; }
        double volume() const override { return 0; }
        string serialize() const override {
            ostringstream oss;
            oss << "rect " << width << " " << height;
            return oss.str();
        }
        static shared_ptr<Rectangle> deserialize(const string& s) {
            istringstream iss(s);
            string type;
            double w, h;
            iss >> type >> w >> h;
            return make_shared<Rectangle>(w, h);
        }
    };

    class Triangle : public Geometry2D {
    private:
        double base;
        double height;
    public:
        Triangle(double b, double h) : base(b), height(h) {}
        string type() const override { return "triangle"; }
        double area() const override { return 0.5 * base * height; }
        double volume() const override { return 0; }
        string serialize() const override {
            ostringstream oss;
            oss << "triangle " << base << " " << height;
            return oss.str();
        }
        static shared_ptr<Triangle> deserialize(const string& s) {
            istringstream iss(s);
            string type;
            double b, h;
            iss >> type >> b >> h;
            return make_shared<Triangle>(b, h);
        }
    };

    class Square : public Geometry2D {
    private:
        double side;
    public:
        Square(double s) : side(s) {}
        string type() const override { return "square"; }
        double area() const override { return side * side; }
        double volume() const override { return 0; }
        string serialize() const override {
            ostringstream oss;
            oss << "square " << side;
            return oss.str();
        }
        static shared_ptr<Square> deserialize(const string& s) {
            istringstream iss(s);
            string type;
            double side;
            iss >> type >> side;
            return make_shared<Square>(side);
        }
    };

    shared_ptr<Geometry2D> Geometry2D::deserialize(const string& s) {
        istringstream iss(s);
        string type;
        iss >> type;
        if (type == "circle") {
            return Circle::deserialize(s);
        }
        else if (type == "rect") {
            return Rectangle::deserialize(s);
        }
        else if (type == "triangle") {
            return Triangle::deserialize(s);
        }
        else if (type == "square") {
            return Square::deserialize(s);
        }
        else {
            return nullptr;
        }
    }
    VFS::VirtualFileSystem vfs;
    int main() {
      /*  vfs.loadFromFile();
        vfs.show(vfs.root);

        return 0;*/

        
        vector<shared_ptr<Geometry2D>> shapes;
        shapes.push_back(make_shared<Circle>(50));
        shapes.push_back(make_shared<Rectangle>(30, 40));
        shapes.push_back(make_shared<Triangle>(20, 60));
        shapes.push_back(make_shared<Square>(70));

        for (const auto& shape : shapes) {
            cout << shape->type() << " area: " << shape->area() << " volume: " << shape->volume() << endl;
            cout << "Serialized: " << shape->serialize() << endl;
            auto deserialized = Geometry2D::deserialize(shape->serialize());
            if (deserialized) {
                cout << "Deserialized: " << deserialized->type() << " area: " << deserialized->area() << " volume: " << deserialized->volume() << endl;
            }
            else {
                cout << "Failed to deserialize" << endl;
            }
        }

        vfs.createFolder("/root", "folder1");
        int cnt = 0;
        for (auto& shape : shapes) {
            string filename =shape->type() + to_string(++cnt);
            vfs.createFile("/root/folder1", filename, "");
            vfs.writeFile("/root/folder1/" + filename, shape->serialize());
        }

        vfs.show(vfs.root);
        vfs.saveToFile();

        return 0;
    }
}