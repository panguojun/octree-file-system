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
        Cube(const shared_ptr<Point>& corner, double length) : corner_(corner), length_(length) {}
        string type() const override { return "Cube"; }
        double area() const override { return 6 * length_ * length_; }
        double volume() const override { return length_ * length_ * length_; }
        string serialize() const override {
            return "Cube " + corner_->serialize() + " " + to_string(length_);
        }
        static shared_ptr<Cube> deserialize(const string& s) {
            auto tokens = STR::split(s, ' ');
            auto corner = Point::deserialize(tokens[1]);
            double length = stod(tokens[2]);
            return make_shared<Cube>(corner, length);
        }
    public:
        shared_ptr<Point> corner_;
        double length_;
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
        return nullptr;
    }

    VFS::VirtualFileSystem vfs;
    int main() {
        shared_ptr<Point> p1 = make_shared<Point>(0, 0, 0);
        shared_ptr<Point> p2 = make_shared<Point>(1, 0, 0);
        shared_ptr<Point> p3 = make_shared<Point>(1, 1, 0);
        shared_ptr<Point> p4 = make_shared<Point>(0, 1, 0);
        shared_ptr<Point> p5 = make_shared<Point>(0, 0, 1);
        shared_ptr<Point> p6 = make_shared<Point>(1, 0, 1);
        shared_ptr<Point> p7 = make_shared<Point>(1, 1, 1);
        shared_ptr<Point> p8 = make_shared<Point>(0, 1, 1);
        shared_ptr<Point> p9 = make_shared<Point>(2, 0, 0);
        shared_ptr<Point> p10 = make_shared<Point>(2, 2, 0);
        shared_ptr<Point> p11 = make_shared<Point>(0, 2, 0);

        shared_ptr<Line> line = make_shared<Line>(p1, p2);
        shared_ptr<Face> face = make_shared<Face>(vector<shared_ptr<Point>>{p1, p2, p3, p4});
        shared_ptr<Cube> cube = make_shared<Cube>(p1, 1.0);

        GeometryTable table;
        table.add(line);
        table.add(face);
        table.add(cube);

        table.query();

        //table.save("geometry.txt");

        // save to vfs
        //VFS::VirtualFileSystem vfs;
        
        vfs.createFolder("/root", "folder1");
        vfs.createFile("/root/folder1", "file1", "");
        vfs.createFile("/root/folder1", "file2", "");
        vfs.createFile("/root/folder1", "file3", "Hello, world!");
        int cnt = 0;
        for (auto& geometry : table.geometries_) {
            vfs.writeFile("/root/folder1/file" + to_string(++cnt), geometry->serialize());
        }

        vfs.show(vfs.root);

       /* vfs.saveToFile();

        GeometryTable table2;
        table2.load("geometry.txt");
        table2.query();*/

        return 0;
    }
}