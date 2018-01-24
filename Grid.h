class Grid{
	private:
		int _mode, _outStyle, _rows, _columns;
		std::string **_currGen;
		double _popDensity;
		std::string _mapFile;	
	
	public:
		bool isStable;
		Grid(int mode, int outStyle, int rows, int columns, double popDensity);
		Grid(int mode, int outStyle,  std::string mapFile);
		~Grid();
		void populateMap();
		void readMap();	
		void calculateNextGen();
		int getNeighborsClassic(int r, int c);
		int getNeighborsDoughnut(int r, int c);
		int getNeighborsMirror(int r, int c);
		bool isEmpty();
		void output(int gen);
		void output(int gen, std::string filename);
};
