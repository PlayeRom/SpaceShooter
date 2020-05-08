/*! \file
	\brief Plik nag��wkowy dla klasy CSingletonBase.
*/

#pragma once

/// Klasa szablonu singleton�w
/** Wszelkie klasy maj�ce by� singletonami powinny dziedziczy� po tej klasie. */
template<typename T> class CSingletonBase
{
public:
	/// Destruktor
	/** Destruktor */
	virtual ~CSingletonBase() { m_pInst.release(); } 

	/// Statyczna metoda za pomoc�, kt�rej pobieramy wska�nik na obiekt klasy szablonu. 
	/*! \return Wska�nik na obiekt klasy szablonu. Je�eli nie wywo�amy wcze�niej Construct to metoda
	GetInstance zwr�ci \p NULL! */
	static inline T* GetInstance() { return m_pInst.get(); }

protected:
	/// Statyczna metoda tworz�ca obiekt klasy szablonu. 
	/** \return Wska�nik na obiekt klasy szablonu. */
	static inline T* Construct( T *pInst ) {
		if( !m_pInst.get() )
			m_pInst.reset( pInst );
		return m_pInst.get();
	}

private:
	static std::auto_ptr<T> m_pInst; ///< Instancja klasy.
};

template<typename T> std::auto_ptr<T> CSingletonBase<T>::m_pInst;