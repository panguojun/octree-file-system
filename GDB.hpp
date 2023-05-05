/**                     【几何体表格】      
*/
#define VLIST(clsname)    vector<shared_ptr<clsname>>
namespace GDB {
    // 几何体类
    class Geometry {
    public:
        virtual string type() const = 0;  // 返回几何体类型的字符串表示
        virtual double area() const = 0;  // 计算几何体的面积
        virtual double volume() const = 0;  // 计算几何体的体积
        virtual string serialize() const = 0;  // 将几何体对象序列化为字符串
        static shared_ptr<Geometry> deserialize(const string& s);  // 从字符串中反序列化出几何体对象
    };

    // 点类
    class Point : public Geometry {
    public:
        Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}
        string type() const override { return "Point"; }
        double area() const override { return 0; }
        double volume() const override { return 0; }
        string serialize() const override {
           // return "point " + to_string(x_) + " " + to_string(y_) + " " + to_string(z_);
            return "circle " + to_string(x_) + "," + to_string(y_) + " 8";
        }
        static shared_ptr<Point> deserialize(const string& s) {
            double x, y, z;
            sscanf(s.c_str(), "Point %lf %lf %lf", &x, &y, &z);
            return make_shared<Point>(x, y, z);
        }
    public:
        double x_, y_, z_;
    };

    // 线段类
    class Line : public Geometry {
    public:
        Line(const shared_ptr<Point>& p1, const shared_ptr<Point>& p2) : p1_(p1), p2_(p2) {}
        string type() const override { return "Line"; }
        double area() const override { return 0; }
        double volume() const override { return 0; }
        string serialize() const override {
           // return "Line " + p1_->serialize() + " " + p2_->serialize();
            return "line " + to_string(int(p1_->x_ * 500)) + "," + to_string(int(p1_->y_ * 500)) + " " + to_string(int(p2_->x_ * 500)) + " " + to_string(int(p2_->y_*500));
        }
        static shared_ptr<Line> deserialize(const string& s) {
            auto tokens = STR::split(s, ' ');
            auto p1 = Point::deserialize(tokens[1]);
            auto p2 = Point::deserialize(tokens[2]);
            return make_shared<Line>(p1, p2);
        }
    public:
        shared_ptr<Point> p1_, p2_;
    };

    // 面类
    class Face : public Geometry {
    public:
        Face(const vector<shared_ptr<Point>>& points) : points_(points) {}
        string type() const override { return "Face"; }
        double area() const override {
            // 计算多边形面积的公式
            int n = points_.size();
            double area = 0;
            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                area += points_[i]->x_ * points_[j]->y_ - points_[j]->x_ * points_[i]->y_;
            }
            return 0.5 * abs(area);
        }
        double volume() const override { return 0; }
        string serialize() const override {
            string s = "Face";
            for (const auto& p : points_) {
                s += " " + p->serialize();
            }
            return s;
        }
        static shared_ptr<Face> deserialize(const string& s) {
            auto tokens = STR::split(s, ' ');
            vector<shared_ptr<Point>> points;
            for (size_t i = 1; i < tokens.size(); i++) {
                points.push_back(Point::deserialize(tokens[i]));
            }
            return make_shared<Face>(points);
        }
    public:
        vector<shared_ptr<Point>> points_;
    };

    // 立方体类
    class Cube : public Geometry {
    public:
        Cube(double length) : length_(length) {}
        string type() const override { return "Cube"; }
        double area() const override { return 6 * length_ * length_; }
        double volume() const override { return length_ * length_ * length_; }
        string serialize() const override {
            return "Cube " + to_string(length_);
        }
        static shared_ptr<Cube> deserialize(const string& s) {
            double length = stof(s);
            return make_shared<Cube>(length);
        }
    public:
        shared_ptr<Point> corner_;
        double length_;
    };
    // 球类
    class Sphere : public Geometry {
    public:
        Sphere(double radius) : radius_(radius) {}
        string type() const override { return "Sphere"; }
        double area() const override { return 4 * PI * radius_ * radius_; }
        double volume() const override { return (4.0 / 3.0) * PI * radius_ * radius_ * radius_; }
        string serialize() const override {
            return "Sphere " + to_string(radius_);
        }
        static shared_ptr<Sphere> deserialize(const string& s) {
            double radius = stof(s);
            return make_shared<Sphere>(radius);
        }
    public:
        shared_ptr<Point> center_;
        double radius_;
    };

    // 圆柱类
    class Cylinder : public Geometry {
    public:
        Cylinder(double radius, double height) : radius_(radius), height_(height) {}
        string type() const override { return "Cylinder"; }
        double area() const override { return 2 * PI * radius_ * height_ + 2 * PI * radius_ * radius_; }
        double volume() const override { return PI * radius_ * radius_ * height_; }
        string serialize() const override {
            return "Cylinder " + to_string(radius_) + " " + to_string(height_);
        }
        static shared_ptr<Cylinder> deserialize(const string& s) {
            auto tokens = STR::split(s, ' ');
            double radius = stod(tokens[1]);
            double height = stod(tokens[2]);
            return make_shared<Cylinder>(radius, height);
        }
    public:
        shared_ptr<Point> bottom_center_;
        double radius_;
        double height_;
    };

    // 圆锥类
    class Cone : public Geometry {
    public:
        Cone(double radius, double height) : radius_(radius), height_(height) {}
        string type() const override { return "Cone"; }
        double area() const override { return PI * radius_ * (radius_ + sqrt(radius_ * radius_ + height_ * height_)); }
        double volume() const override { return (1.0 / 3.0) * PI * radius_ * radius_ * height_; }
        string serialize() const override {
            return "Cone " + to_string(radius_) + " " + to_string(height_);
        }
        static shared_ptr<Cone> deserialize(const string& s) {
            auto tokens = STR::split(s, ' ');
            double radius = stod(tokens[1]);
            double height = stod(tokens[2]);
            return make_shared<Cone>(radius, height);
        }
    public:
        shared_ptr<Point> bottom_center_;
        double radius_;
        double height_;
    };

    // 圆环类
    class Torus : public Geometry {
    public:
        Torus(double major_radius, double minor_radius) : major_radius_(major_radius), minor_radius_(minor_radius) {}
        string type() const override { return "Torus"; }
        double area() const override { return 4 * PI * PI * major_radius_ * minor_radius_; }
        double volume() const override { return 2 * PI * PI * minor_radius_ * minor_radius_ * major_radius_; }
        string serialize() const override {
            return "Torus " + to_string(major_radius_) + " " + to_string(minor_radius_);
        }
        static shared_ptr<Torus> deserialize(const string& s) {
            auto tokens = STR::split(s, ' ');
            double major_radius = stod(tokens[1]);
            double minor_radius = stod(tokens[2]);
            return make_shared<Torus>( major_radius, minor_radius);
        }
    public:
        shared_ptr<Point> center_;
        double major_radius_;
        double minor_radius_;
    };

    // 扇面类
    class Sector : public Geometry {
    public:
        Sector(double radius, double angle) : radius_(radius), angle_(angle) {}
        string type() const override { return "Sector"; }
        double area() const override { return 0.5 * radius_ * radius_ * angle_; }
        double volume() const override { return 0; } // 扇面没有体积
        string serialize() const override {
            return "Sector " + to_string(radius_) + " " + to_string(angle_);
        }
        static shared_ptr<Sector> deserialize(const string& s) {
            auto tokens = STR::split(s, ' ');
            double radius = stod(tokens[1]);
            double angle = stod(tokens[2]);
            return make_shared<Sector>( radius, angle);
        }
    public:
        shared_ptr<Point> center_;
        double radius_;
        double angle_; // 弧度制
    };

    // 棱柱类
    class Prism : public Geometry {
    public:
        Prism(const vector<shared_ptr<Point>>& vertices, double height) : vertices_(vertices), height_(height) {}
        string type() const override { return "Prism"; }
        double area() const override { return 0; } // 棱柱没有表面积
        double volume() const override { return height_ * base_area_; }
        string serialize() const override {
            string s = "Prism " + to_string(vertices_.size()) + " ";
            for (auto& vertex : vertices_) {
                s += vertex->serialize() + " ";
            }
            s += to_string(height_);
            return s;
        }
        static shared_ptr<Prism> deserialize(const string& s) {
            auto tokens = STR::split(s, ' ');
            int num_vertices = stoi(tokens[1]);
            vector<shared_ptr<Point>> vertices;
            for (int i = 2; i < 2 + num_vertices; i++) {
                vertices.push_back(Point::deserialize(tokens[i]));
            }
            double height = stod(tokens[2 + num_vertices]);
            return make_shared<Prism>(vertices, height);
        }
        void set_base_area(double area) { base_area_ = area; }
    public:
        vector<shared_ptr<Point>> vertices_;
        double height_;
        double base_area_;
    };
    // 几何体表格类
    class GeometryTable {
    public:
        void add(const shared_ptr<Geometry>& geometry) {
            geometries_.push_back(geometry);
        }
        void query() const {
            for (const auto& geometry : geometries_) {
                cout << geometry->type() << " Area: " << geometry->area() << " Volume: " << geometry->volume() << endl;
            }
        }
        void save(const string& filename) const {
            ofstream fout(filename);
            if (!fout) {
                cerr << "Error: Failed to open file " << filename << endl;
                return;
            }
            for (const auto& geometry : geometries_) {
                fout << geometry->serialize() << endl;
            }
            fout.close();
        }
        void load(const string& filename) {
            ifstream fin(filename);
            if (!fin) {
                cerr << "Error: Failed to open file " << filename << endl;
                return;
            }
            string line;
            while (getline(fin, line)) {
                auto geometry = Geometry::deserialize(line);
                if (geometry) {
                    geometries_.push_back(geometry);
                }
            }
            fin.close();
        }
    public:
        vector<shared_ptr<Geometry>> geometries_;

        static vector<string> split(const string& s, char delimiter) {
            vector<string> tokens;
            size_t start = 0, end = 0;
            while ((end = s.find(delimiter, start)) != string::npos) {
                tokens.push_back(s.substr(start, end - start));
                start = end + 1;
            }
            tokens.push_back(s.substr(start));
            return tokens;
        }
    };

    shared_ptr<Geometry> Geometry::deserialize(const string& s) {
        auto tokens = GeometryTable::split(s, ' ');
        if (tokens.empty()) {
            return nullptr;
        }
        if (tokens[0] == "Point") {
            return Point::deserialize(s);
        }
        if (tokens[0] == "Line") {
            return Line::deserialize(s);
        }
        if (tokens[0] == "Face") {
            return Face::deserialize(s);
        }
        if (tokens[0] == "Cube") {
            return Cube::deserialize(s);
        }
        if (tokens[0] == "Cylinder") {
            return Cylinder::deserialize(s);
        }
        if (tokens[0] == "Sphere") {
            return Sphere::deserialize(s);
        }
        if (tokens[0] == "Cone") {
            return Cone::deserialize(s);
        }
        if (tokens[0] == "Torus") {
            return Torus::deserialize(s);
        }
        if (tokens[0] == "Sector") {
            return Sector::deserialize(s);
        }
        if (tokens[0] == "Prism") {
            return Prism::deserialize(s);
        }
        return nullptr;
    }

    VFS::VirtualFileSystem vfs;
    int main() {
        /*vfs.loadFromFile();
        vfs.show(vfs.root);

        return 0;*/


        vector<shared_ptr<Geometry>> shapes;
        // 创建不同的几何体
        shapes.push_back(make_shared<Cylinder>(3, 4));
        shapes.push_back(make_shared<Cone>( 2, 6));
        shapes.push_back(make_shared<Torus>(7, 1));
        shapes.push_back(make_shared<Sector>(1, 1.5));

        for (const auto& shape : shapes) {
            cout << shape->type() << " area: " << shape->area() << " volume: " << shape->volume() << endl;
            cout << "Serialized: " << shape->serialize() << endl;
            auto deserialized = Geometry::deserialize(shape->serialize());
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
            string filename = shape->type() + to_string(++cnt);
            vfs.createFile("/root/folder1", filename, "");
            vfs.writeFile("/root/folder1/" + filename, shape->serialize());
        }

        vfs.show(vfs.root);
        vfs.saveToFile();

        return 0;
    }
}