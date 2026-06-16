#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        // 1. VISUAL GRIEF LIMPÍSSIMO
        if (this->m_background) this->m_background->setColor({15, 0, 0});
        if (this->m_groundLayer) this->m_groundLayer->setColor({5, 0, 0});

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto redFilter = CCLayerColor::create({150, 0, 0, 50}); // Filtro vermelho suave
        redFilter->setZOrder(100); 
        this->addChild(redFilter);

        // Vetor para guardar os objetos novos e adicionar depois (evita bugs na fase)
        std::vector<GameObject*> novosObjetos;

        // 2. ADICIONANDO NOVOS OBSTÁCULOS BONITINHOS
        if (this->m_objects) {
            for (int i = 0; i < this->m_objects->count(); i++) {
                auto obj = static_cast<GameObject*>(this->m_objects->objectAtIndex(i));
                if (obj) {
                    
                    // Pinta os objetos originais para manter o tema
                    if (obj->m_objectType == GameObjectType::Hazard) {
                        obj->setColor({255, 30, 30}); 
                    } else {
                        obj->setColor({40, 10, 10});
                    }

                    // Tática 1: Alargar os saltos perfeitos
                    // Se achar um espinho (ID 8 é o espinho padrão do jogo)
                    if (obj->m_objectID == 8) { 
                        // A cada 2 espinhos, nós criamos um novo para fechar o espaço
                        if (i % 2 == 0) {
                            auto espinhoExtra = GameObject::createWithKey(8); 
                            if (espinhoExtra) {
                                // Coloca o novo espinho 30 pixels à frente do original
                                espinhoExtra->setPosition({obj->getPositionX() + 30.0f, obj->getPositionY()});
                                espinhoExtra->setRotation(obj->getRotation());
                                espinhoExtra->setColor({255, 30, 30});
                                novosObjetos.push_back(espinhoExtra);
                            }
                        }
                    }
                    
                    // Tática 2: Teto baixo com espinhos invertidos
                    // Adiciona um espinho flutuante acima dos blocos sólidos para dificultar o pulo
                    if (obj->m_objectType == GameObjectType::Solid) {
                         // Adiciona apenas em alguns blocos para não lotar a tela (a cada 4 blocos)
                         if (i % 4 == 0) {
                             auto tetoEspinho = GameObject::createWithKey(8);
                             if (tetoEspinho) {
                                 // Coloca 90 pixels acima do bloco, de cabeça para baixo
                                 tetoEspinho->setPosition({obj->getPositionX(), obj->getPositionY() + 90.0f});
                                 tetoEspinho->setRotation(180.0f); // Inverte o espinho
                                 tetoEspinho->setColor({255, 30, 30});
                                 novosObjetos.push_back(tetoEspinho);
                             }
                         }
                    }
                }
            }
        }

        // 3. INSERE OS OBJETOS NOVOS NA FASE
        // Agora nós pegamos todos os espinhos extras que criamos e injetamos na fase real
        for (auto novoObj : novosObjetos) {
            this->m_objectLayer->addChild(novoObj);
            this->m_objects->addObject(novoObj); // Garante que a colisão funcione
        }

        return true;
    }
};
