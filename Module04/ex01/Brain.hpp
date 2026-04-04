#ifndef BRAIN_HPP
# define BRAIN_HPP

# include <string>

class Brain {

private:
	std::string ideas[100];

public:
	// OCF
	Brain();
    Brain(const Brain& src);
    Brain& operator=(const Brain& rhs);
    ~Brain();

	void setIdea(int index, const std::string& idea);
    std::string getIdea(int index) const;
};

#endif

//シャローコピー: アドレスだけをコピー -> 2匹の犬が1つの脳を共有する状態
    // -> 一方が死ぬと、もう一方の犬の脳も消え、さらにその犬が死ぬ時に「すでに消されたメモリ」を二重に解放（Double Free）
//ディープコピー: new Brain() を確保
