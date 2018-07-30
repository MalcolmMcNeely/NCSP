using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

using MHelper;

namespace MControls
{
   /// <summary>
   /// Validation rule for entering positive numbers only
   /// </summary>
   public class PositiveRangeNumberValidationRule : ValidationRule
   {
      public IntPair _validRange = null;
      public IntPair ValidRange
      {
         get { return _validRange; }
         set { _validRange = value; }
      }

      public override ValidationResult Validate(object value, CultureInfo cultureInfo)
      {
         String errorMessage;
         int input;
         bool notZero = true;
         bool inRange = true;
         bool isNumber = int.TryParse(value.ToString(), out input);

         if (isNumber)
         {
            if ((int)value > 0)
            {
               if (_validRange != null)
               {
                  inRange = (input >= _validRange.Minimum && input <= _validRange.Maximum);
               }
            }
            else
            {
               notZero = false;
               errorMessage = "Input must be greater than 0";
            }
         }

         errorMessage = !isNumber ? "Input must be a number" : !notZero ? "Input must be greater than 0" : 
            !inRange ? "Input needs to be between " + _validRange.Minimum + " and " + _validRange.Maximum : String.Empty;
         return new ValidationResult(notZero && inRange, errorMessage);
      }
   }
}
