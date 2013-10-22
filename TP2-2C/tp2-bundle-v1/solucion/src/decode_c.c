void decode_c(unsigned char* src,
              unsigned char* code,
			  int size,
              int width,
              int height)
{

	unsigned char pixelActual;
	unsigned char modoDecode;
	unsigned char valorDecode;
	unsigned int indiceW;
	unsigned int cantBytes = (width * height * 3);
	unsigned int indicePixel;

		for (indiceW = 0, indicePixel = 0; (indiceW < (size - 1) && (indicePixel + 4 < cantBytes)); indicePixel+=4, indiceW++, code++)
		{
			unsigned char nuevoValor = 0;
			
			for (unsigned char numeroDePar = 0; numeroDePar < 4; numeroDePar++, src++)
			{
				pixelActual = *src;
				modoDecode = pixelActual % 16;
				modoDecode = modoDecode >> 2;
				valorDecode = pixelActual % 4;


				if (modoDecode == 1)
				{
					valorDecode++;
					valorDecode = valorDecode % 4;
				}
				else
				{
					if (modoDecode == 2)
					{
						valorDecode--;
						valorDecode = valorDecode % 4;
					}
					else
					{
						if (modoDecode == 3)
						{
							valorDecode = (~valorDecode);
							valorDecode = valorDecode % 4;
						}
					}
				}

				unsigned char shift = 2*numeroDePar;
				valorDecode = valorDecode << shift;
				nuevoValor = nuevoValor + valorDecode;
			}
			//nuevoValor = 0x55
			*code = nuevoValor;
		}
	
}
